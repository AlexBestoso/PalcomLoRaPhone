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
	}
}

void PalcomSettings::update(void){
	uint8_t buffer[20];
	buffer[0] = data.encryption;
	buffer[1] = data.encoding;
	buffer[2] = data.send_route;
	buffer[3] = data.recv_route;
	for(int i=0; i<8; i++){
		buffer[4+i] = data.pre[i];
		buffer[4+8+i] = data.app[i];
	}

	if(!this->partition.write(buffer, 20)){
	}else{
	}
        for(int i=0; i<8; i++){
                buffer[4+i] = data.pre[i];
                buffer[4+8+i] = data.app[i];
	}
}

palcom_partition_t PalcomSettings::getPartition(bool reload){
	if(reload) this->load();
	return this->data;
}
void PalcomSettings::setEncryption(const char *encMode){
	if(encMode == NULL) return;

	String m = encMode;
	
	if(m == "Disabled") 	this->data.encryption = 0;
	else if(m == "AES-XTS") this->data.encryption = 1;
	else if(m == "AES-OFB") this->data.encryption = 2;
	else if(m == "AES-CTR") this->data.encryption = 3;
	else if(m == "AES-ECB") this->data.encryption = 4;
	else if(m == "AES-CBC") this->data.encryption = 5;
	else this->data.encryption = 0;
}
void PalcomSettings::setEncoding(const char * encMode){
	if(encMode == NULL) return;
	String m = encMode;
	if(m == "Disabled") 	this->data.encoding = 0;
	else if(m == "Base64") 	this->data.encoding = 1;
}
void PalcomSettings::setSendRoute(const char *route){
	if(route == NULL) return;
	String m = route;
	if(m == "Usb") this->data.send_route = 0;
	else if(m == "LoRa") this->data.send_route = 1;
	else if(m == "WiFi") this->data.send_route = 2;
}
void PalcomSettings::setRecvRoute(const char *route){
	if(route == NULL) return;
	String m = route;
	if(m == "Usb") this->data.recv_route = 0;
	else if(m == "LoRa") this->data.recv_route = 1;
	else if(m == "WiFi") this->data.recv_route = 2;

}
void PalcomSettings::setPrepend(const char *str){
	if(str == NULL) return;

	for(int i=0, j=0; i<8; i++){
		if(str[j] == 0x00){
			this->data.pre[i] = 0x00;
		}else{
			this->data.pre[i] = str[j];
			j++;
		}
	}
}
void PalcomSettings::setAppend(const char *str){
	if(str == NULL) return;

	for(int i=0, j=0; i<8; i++){
		if(str[j] == 0x00){
			this->data.app[i] = 0x00;
		}else{
			this->data.app[i] = str[j];
			j++;
		}
	}

}
