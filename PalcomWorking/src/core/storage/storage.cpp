#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <SD.h>
#include <src/taskQueue/taskQueue.h>
#include "./storage.h"

extern SemaphoreHandle_t xSemaphore;
extern TaskQueue taskQueue;
extern bool sd_card_available;
extern char displayed_messages[10][257];
extern int displayed_page;
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


int Storage::getMsgCount(void){
	return this->getCacheSize()/257;
}
int Storage::getCacheSize(void){
	int ret = 0;
	if(SD.exists("/msgCache")){
                this->_fd = SD.open("/msgCache", FILE_READ);
                        ret = this->_fd.size();
                this->_fd.close();
        }
	return ret;
}

bool Storage::storeMessage(bool mine){
	if(!sd_card_available){
		Serial.printf("no sd card, temporary msgs.\n");
		this->relayMsgToDisplay(mine);
		return false;
	}
	
	// debugging
	size_t cacheSize = this->getCacheSize();
	if(SD.exists("/msgCache")){
		//this->_fd = SD.open("/msgCache", FILE_READ);
			//cacheSize = this->_fd.size();
//		this->_fd.close();
	}
	Serial.printf("Message Cache Size %ld \t %ld\n", cacheSize, cacheSize/257);
	
	/*size_t msgCount = 0;
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
	}*/

	char msgCacheBuffer[257];
	memset(msgCacheBuffer, 'a', 257);
	msgCacheBuffer[0] = mine ? 1 : 0;

	for(int i=0; i<256; i++){
		msgCacheBuffer[i+1] = this->task.msg[i];
	}

	Serial.printf("Store Debug: ");
	for(int i=0; i<257; i++){
		Serial.printf("%c", msgCacheBuffer[i]);
	}
	Serial.printf("\n");
	this->_fd = SD.open("/msgCache", FILE_WRITE);
		this->_fd.seek(cacheSize);
		if(this->_fd.write((uint8_t *)msgCacheBuffer, 257) != 257)
			Serial.printf("[E] Partial data storage\n");
		else
			Serial.printf("Message stored.\n");
	this->_fd.close();
//	cacheSize+=257;
	
//	msgCount = cacheSize/257;
//	String conv = String(msgCount);

//	SD.remove("/msgCount");
//	this->_fd = SD.open("/msgCount", FILE_WRITE);
//		this->_fd.write((uint8_t*)conv.c_str(), conv.length());
//	this->_fd.close();

	this->relayMsgToDisplay(mine);
	return true;
}

bool Storage::refreshMsg(void){
	int msgCount = this->getMsgCount();
	for(int i=0; i<10; i++) displayed_messages[i][0] = 2;
	if(msgCount <= 0) {
		return false;
	}else if(msgCount <= 10){
		this->_fd = SD.open("/msgCache", FILE_READ);
			int track = 0;
			for(int i=0; i<this->_fd.size() && track < 10; i+=257){
				
				if(this->_fd.seek(i) == 0){
					Serial.printf("[E] Storage Read Seek Error.\n");
				}
				Serial.printf("Positiion : %d\t", this->_fd.position());
				char buf[257];
				if(this->_fd.read((uint8_t*)buf, 257) == -1){
					Serial.printf("[E] Storage Read error.\n");
				}
				for(int j=0; j<257; j++){
					displayed_messages[track][j] = buf[j];
					Serial.printf("%c", buf[j]);
				}
				Serial.printf("\n");
				track++;
			}
		this->_fd.close();
	}else{ // msg count is > 10
		Serial.printf("fetching paginated results.\n");	
		this->_fd = SD.open("/msgCache", FILE_READ);
			int track = 9;
			size_t fdSize = this->_fd.size();
			for(int i=(fdSize-1)-257; i>-1 && track > -1; i-=257){
				if(this->_fd.seek(i) == 0){
					Serial.printf("[E] Storage read seek error.\n");
				}
				Serial.printf("Positiion : %d\t", this->_fd.position());
				char buf[257];
				if(this->_fd.read((uint8_t*)buf, 257) == -1){
                                        Serial.printf("[E] Storage Read error.\n");
                                }

				for(int j=0; j<257; j++){
					displayed_messages[track][j] = buf[j];
					Serial.printf("%c", buf[j]);
				}
				Serial.printf("\n");
                                track--;
			}
		this->_fd.close();
	}
	
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
		case STORAGE_INSTR_REFRESH_MSG:
			return this->refreshMsg();
	}
	return false;
}
