#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <src/taskQueue/taskQueue.h>
#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>
#include <src/PalcomObject/PalcomObject.h>
#include <src/LoRaSnake/LoRaSnake.class.h>

#include "./comms.h"

extern TaskQueue taskQueue;
extern LoRaSnake loraSnake;
extern int palcome_message_mode;
extern SemaphoreHandle_t xSemaphore;

//Private
bool Comms::pop(void){
        struct task_queue_task t = taskQueue.pop(this->spaceType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool Comms::sendMsgNode(void){
	Serial.printf("Comms::sendMsg - Node not Configured!\n");
	return true;
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
bool Comms::recvMsg(void){
	if(xSemaphore == NULL){
                Serial.printf("Comms::sendMsg - Semaphore is null\n");
                return false;
        }else if(xSemaphoreTake(xSemaphore, portMAX_DELAY) != pdTRUE){
                return false;
        }
	bool ret = false;
	Serial.printf("Processing recv task.\n");
	if(loraSnake.readRecv() == 1){
		struct task_queue_task tmp;
		tmp.to = TASK_SPACE_STORAGE;
		tmp.from = TASK_SPACE_COMMS;
		tmp.active = true;
		tmp.instruction = STORAGE_INSTR_RECVED;
		memset(tmp.msg, '\0', 256);
		for(int i=0; i<loraSnake.lrsPacket.data_size && i < 256; i++)
			tmp.msg[i] = loraSnake.lrsPacket.data[i];
		
		Serial.printf("Pushing storage task...\n");
		taskQueue.push(tmp);
		Serial.printf("Message storage queued. - ");		

		Serial.printf("%ld bytes : \n\t", loraSnake.lrsPacket.data_size);
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
        }
        return false;
}
