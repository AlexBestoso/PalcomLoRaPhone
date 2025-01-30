#include <Arduino.h>
#include <cstdint>
#include <SD.h>

#include "./SdSnake.h"

void SdSnake::rm(const char *target){
	if(!SD.exists(target))
		return;
	File root = SD.open(target);
	if(root.isDirectory()){
		while(true){
			String name = target;
			File node = root.openNextFile();
			if(!node) break;

			if(name != "/") name += "/";

			name += node.name();
			
			if(node.isDirectory()){
				this->rm(name.c_str());
				node.close();
				if(!SD.rmdir(name.c_str()))
					Serial.printf("Failed to remove directory %s\n", name.c_str());
			}else{
				node.close();
				SD.remove(name.c_str());
			}
		}
		root.close();
		if(!SD.rmdir(target))
		Serial.printf("Failed to remove directory %s\n", (const char *)target);
	}else{
		root.close();
		SD.remove(target);
	}
}

bool SdSnake::writeFile(const char *fname, char *data, size_t dataSize){
	return true;
}
bool SdSnake::readFile(const char *fname, char *outData, size_t outDataSize){
	return true;
}
bool SdSnake::init(void){
	return true;
}

