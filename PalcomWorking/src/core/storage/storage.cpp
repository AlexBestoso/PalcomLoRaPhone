#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <SD.h>
#include <src/taskQueue/taskQueue.h>
#include "./storage.h"

extern SemaphoreHandle_t xSemaphore;
extern TaskQueue taskQueue;
//Private
bool Storage::pop(void){
	struct task_queue_task t = taskQueue.pop(this->spaceType);
	if(!t.active)
		return false;
	this->task = t;
	return true;
}

bool Storage::storeMessage(void){
	
	return true;
}

//Public
Storage::Storage(void){
	this->spaceType = TASK_SPACE_STORAGE;
}
Storage::~Storage(){}

bool Storage::fetchTask(void){
	return this->pop();
}

bool Storage::runTask(void){
	switch(this->task.instruction){
		case COMMS_INSTR_PUSH_MSG:
			return this->storeMessage;
	}
	return false;
}
