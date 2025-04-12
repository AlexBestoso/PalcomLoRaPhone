#include <Arduino.h>
#include <cstdint>

#include <src/error/error.h>

#include <src/taskQueue/taskQueue.h>

#include "./settings.h"

PalcomSettings::PalcomSettings(void){
	
}

PalcomSettings::~PalcomSettings(){
	
}

void PalcomSettings::load(void){
	bool good = partition.read(&this->data);
	if(!good){
		Serial.printf("Failed to load parition data.\n");
	}else{
		Serial.printf("Parition Loaded.\n");
	}

}
