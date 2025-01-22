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

//Private
bool Comms::pop(void){
        struct task_queue_task t = taskQueue.pop(this->spaceType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool Comms::sendMsg(void){
	bool ret = false;
	size_t msgSize = strlen((const char *)this->task.msg);
	msgSize = msgSize > 256 ? 256 : msgSize;
	if(!loraSnake.send(this->task.msg, msgSize)){
      		Serial.printf("Failed to send message.\n");
    	}else{
      		Serial.printf("Sent : [%d] \n", msgSize);
		ret = true;
    	}
	/*Serial.printf("Starting listener.\n");
    	if(!loraSnake.listenStart()){
		Serial.printf("not listening...\n");
	}else{
		Serial.printf("Listener enabled\n");
	}*/
	

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
			return true;
		break;
        }
        return false;
}
