#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <SD.h>
#include <src/taskQueue/taskQueue.h>
#include "./storage.h"

extern SemaphoreHandle_t xSemaphore;
extern TaskQueue taskQueue;
extern bool sd_card_available;
//Private
bool Storage::pop(void){
	struct task_queue_task t = taskQueue.pop(this->spaceType);
	if(!t.active)
		return false;
	this->task = t;
	return true;
}

void Storage::relayMsgToDisplay(bool mine){
	struct task_queue_task tmp;
        tmp.to = TASK_SPACE_GRAPHICS;
        tmp.from = TASK_SPACE_STORAGE;
        tmp.active = true;
        tmp.instruction = mine ? GRAPHICS_INSTR_PUSH_MSGM : GRAPHICS_INSTR_PUSH_MSGO;
	for(int i=0; i<256; i++)
		tmp.msg[i] = this->task.msg[i];
	Serial.printf("Pushing to graphics task...\n");
        taskQueue.push(tmp);
}

bool Storage::storeMessage(bool mine){
	if(!sd_card_available){
		Serial.printf("no sd card, temporary msgs.\n");
		this->relayMsgToDisplay(mine);
		return false;
	}
	
	// debugging
	size_t cacheSize = 0;
	if(SD.exists("/msgCache")){
		this->_fd = SD.open("/msgCache", FILE_READ);
			Serial.printf("Message Cache Size %ld \t %ld\n", this->_fd.size(), this->_fd.size()/257);
			cacheSize = this->_fd.size();
		this->_fd.close();
	}
	
	size_t msgCount = 0;
	char msgCountBuffer[7];
	if(SD.exists("/msgCount")){
		memset(msgCountBuffer, '\0', 7);
		this->_fd = SD.open("/msgCount", FILE_READ);
			size_t fSize = this->_fd.size();
			fSize = fSize >= 8 ? 7 : fSize;
			this->_fd.read((uint8_t*)msgCountBuffer, fSize);
		this->_fd.close();
		String conv = (const char *)msgCountBuffer;
		msgCount = conv.toInt();

		Serial.printf("Detected %d messages\n", (int)msgCount);	
	}

	char msgCacheBuffer[257];
	memset(msgCacheBuffer, 0x00, 257);
	msgCacheBuffer[0] = mine ? 1 : 0;

	for(int i=0; i<256; i++)
		msgCacheBuffer[i+1] = this->task.msg[i];

	this->_fd = SD.open("/msgCache", FILE_WRITE);
		this->_fd.seek(cacheSize);
		this->_fd.write((uint8_t *)msgCacheBuffer, 257);
	this->_fd.close();
	cacheSize+=257;
	
	msgCount = cacheSize/257;
	String conv = String(msgCount);

	SD.remove("/msgCount");
	this->_fd = SD.open("/msgCount", FILE_WRITE);
		this->_fd.write((uint8_t*)conv.c_str(), conv.length());
	this->_fd.close();

	this->relayMsgToDisplay(mine);
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
		case STORAGE_INSTR_RECVED:
			return this->storeMessage(false);
		case STORAGE_INSTR_SENT:
			return this->storeMessage(true);
	}
	return false;
}
