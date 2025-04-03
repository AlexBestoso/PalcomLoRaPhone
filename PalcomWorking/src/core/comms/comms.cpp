#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>

#include <src/taskQueue/taskQueue.h>
#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>
#include <src/PalcomObject/PalcomObject.h>
#include <src/LoRaSnake/LoRaSnake.class.h>
#include <src/cryptography/cryptography.h>

#include "./comms.h"

extern TaskQueue taskQueue;
extern LoRaSnake loraSnake;
extern int palcome_message_mode;
extern SemaphoreHandle_t xSemaphore;

//Private
void Comms::deriveRoutingIv(void){
        for(int i=0, j=0; i<core_routing_key_size && i+1<core_routing_key_size && j<16; i+=2, j++){
                this->iv[j] = core_routing_key[i] ^ core_routing_key[i+1];
        }
}

void Comms::deriveRoutingPadding(void){
        for(int i=0, j=core_routing_key_size-1, q=core_routing_key_size/2, p=(core_routing_key_size/2)-1; (i<=q) && (j >= p); i++, j--, q--, p++){
                int a = core_routing_key[i];
                int b = core_routing_key[j];
                int c = core_routing_key[p];
                int d = core_routing_key[q];

                int big = 0b1111<<4;
                int small = 0b1111;
                this->padding[i] = (a&small) + ((d&small)<<4);
                this->padding[j] = (b&big) + ((c&big)>>4);
                this->padding[p] = (c&small) + ((b&small)<<4);
                this->padding[q] = (d&big) + ((a&big)>>4);
        }
}

void Comms::deriveRoutingRecvKey(void){
        for(int i=0; i<core_routing_key_size; i++)
                this->inKey[i] = core_routing_key[i]; // this should be the inverse of the relay code
}
void Comms::deriveRoutingSendKey(void){
        for(int i=core_routing_key_size-1, j=0; i>0; i--, j++)
                this->outKey[j] = core_routing_key[i]; // This should be the inverse of the relay code.
}

bool Comms::pop(void){
        struct task_queue_task t = taskQueue.pop(this->spaceType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool Comms::sendMsgNode(void){
	if(xSemaphore == NULL){
                Serial.printf("Comms::sendMsg - Semaphore is null\n");
                return false;
        }else if(xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE){
                return false;
        }
        loraSnake.modeSend();

	deriveRoutingIv();
	this->deriveRoutingRecvKey();
        this->deriveRoutingSendKey();
	
        struct task_queue_task tmp;
        bool ret = false;
        size_t msgSize = 256;//strlen((const char *)this->task.msg);
	this->cryptography->setAesMode(CRYPTOGRAPHY_AES_MODE_CBC);
	this->cryptography->setState(this->task.msg, 256);
	this->cryptography->setAesKey((uint8_t *)this->outKey, 32, NULL, 32);
	this->cryptography->setAesIv((uint8_t *)this->iv, 16);
	this->cryptography->aesEncrypt(tmp.msg);

        if(!loraSnake.send(tmp.msg, msgSize)){
                Serial.printf("Failed to send message.\n");
        }else{
                Serial.printf("Sent : [%d] \n", msgSize);
		tmp.to = TASK_SPACE_STORAGE;
        	tmp.from = TASK_SPACE_COMMS;
        	tmp.active = true;
        	tmp.instruction = STORAGE_INSTR_SENT;

		for(int i=3; i<256; i++)
	                tmp.msg[i-3] = this->task.msg[i];
        	taskQueue.push(tmp);
                ret = true;
        }

        loraSnake.modeRecv();

        if(!loraSnake.listenStart()){
                Serial.printf("Failed to start listener.\n");
        }
        xSemaphoreGive( xSemaphore );
        return ret;
}

bool Comms::sendMsg(void){
	if(xSemaphore == NULL){
       		Serial.printf("Comms::sendMsg - Semaphore is null\n");
        	return false;
        }else if(xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE){
		return false;
	}
	loraSnake.modeSend();

	bool ret = false;
	size_t msgSize = strlen((const char *)this->task.msg);
	msgSize = msgSize > 256 ? 256 : msgSize;
	if(!loraSnake.send(this->task.msg, msgSize)){
      		Serial.printf("Failed to send message.\n");
    	}else{
      		Serial.printf("Sent : [%d] \n", msgSize);
		struct task_queue_task tmp;
                tmp.to = TASK_SPACE_STORAGE;
                tmp.from = TASK_SPACE_COMMS;
                tmp.active = true;
                tmp.instruction = STORAGE_INSTR_SENT;
                memset(tmp.msg, '\0', 256);
		for(int i=0; i<msgSize; i++)
			tmp.msg[i] = this->task.msg[i];
		taskQueue.push(tmp);
		
		ret = true;
    	}
	
	loraSnake.modeRecv();

    	if(!loraSnake.listenStart()){
		Serial.printf("Failed to start listener.\n");
	}
	xSemaphoreGive( xSemaphore );
	return ret;
}

bool Comms::recvMsgNode(void){
	if(xSemaphore == NULL){
                Serial.printf("Comms::recvMsgNode - Semaphore is null\n");
                return false;
        }else if(xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE){
                return false;
        }

	deriveRoutingIv();
	this->deriveRoutingRecvKey();
        this->deriveRoutingSendKey();
	
        struct task_queue_task tmp = taskQueue.buildTask(TASK_SPACE_STORAGE, TASK_SPACE_COMMS, STORAGE_INSTR_RECVED);
        bool ret = false;
        if(loraSnake.readRecv() != 1){
		xSemaphoreGive(xSemaphore);
		return false;
	}
	this->cryptography->setAesMode(CRYPTOGRAPHY_AES_MODE_CBC);
        this->cryptography->setState((uint8_t *)loraSnake.lrsPacket.data, 256);
        this->cryptography->setAesKey((uint8_t *)this->inKey, 32, NULL, 32);
        this->cryptography->setAesIv((uint8_t *)this->iv, 16);
        this->cryptography->aesDecrypt(tmp.msg);
	
	if(tmp.msg[0] == 'P' && tmp.msg[1] == 'A' && tmp.msg[2] == 'L')
	        taskQueue.push(tmp);
	else
		Serial.printf("Corrupted lora message.\n");

        xSemaphoreGive( xSemaphore );
        return ret;
}
bool Comms::recvMsg(void){
	if(xSemaphore == NULL){
                Serial.printf("Comms::sendMsg - Semaphore is null\n");
                return false;
        }else if(xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE){
                return false;
        }

	
	bool ret = false;
	if(loraSnake.readRecv() == 1){
		struct task_queue_task tmp;
		tmp.to = TASK_SPACE_STORAGE;
		tmp.from = TASK_SPACE_COMMS;
		tmp.active = true;
		tmp.instruction = STORAGE_INSTR_RECVED;
		memset(tmp.msg, '\0', 256);
		for(int i=0; i<loraSnake.lrsPacket.data_size && i < 256; i++)
			tmp.msg[i] = loraSnake.lrsPacket.data[i];
		
		taskQueue.push(tmp);

	}
	xSemaphoreGive( xSemaphore );
	return ret;
}

//Public
Comms::Comms(void){
        this->spaceType = TASK_SPACE_COMMS;
	this->activePage = 1;
}
Comms::~Comms(){}


bool Comms::fetchTask(void){
        return this->pop();
}

bool Comms::runTask(void){
        switch(this->task.instruction){
                case COMMS_INSTR_SEND:
			return this->sendMsg();
                break;
		case COMMS_INSTR_RECV:
			return this->recvMsg();
		break;
		case COMMS_INSTR_SEND_NODE:
			return this->sendMsgNode();
		break;
		case COMMS_INSTR_RECV_NODE:
			return this->recvMsgNode();
		break;
        }
        return false;
}

void Comms::init(Cryptography *c, unsigned char*core_routing_key, size_t core_routing_key_size){
        this->core_routing_key = core_routing_key;
        this->core_routing_key_size = core_routing_key_size;

        this->cryptography = c;
        this->deriveRoutingIv();
        this->deriveRoutingRecvKey();
        this->deriveRoutingSendKey();
        this->deriveRoutingPadding();
}
