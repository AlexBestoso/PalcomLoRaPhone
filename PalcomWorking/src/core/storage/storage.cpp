#include <Arduino.h>
#include <cstdint>
#include "../../taskQueue/taskQueue.h"
#include "./storage.h"

extern TaskQueue taskQueue;
//Private
bool Storage::pop(void){
	struct task_queue_task t = taskQueue.pop(this->spaceType);
	if(!t.active)
		return false;
	this->task = t;
	return true;
}

//Public
Storage::Storage(void){
	this->spaceType = TASK_SPACE_STORAGE;
}
Storage::~Storage(){}

void Storage::initalize(void){
	Serial.printf("Initalizeing file system.\n");
}

bool Storage::fetchTask(void){
	return this->pop();
}

bool Storage::runTask(void){
	switch(this->task.instruction){
		case STORAGE_INSTR_INITALIZE:
			this->initalize();
			return true;
		break;
	}
	return false;
}
