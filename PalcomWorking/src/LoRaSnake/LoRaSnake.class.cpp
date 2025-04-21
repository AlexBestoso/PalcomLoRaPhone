#include <src/taskQueue/taskQueue.h>
#include "./LoRaSnake.class.h"

extern bool loraSnakeTransmit;
extern bool loraSnakeReceive;
extern SX1262 _radio;
extern TaskQueue taskQueue;
extern int palcome_message_mode;

static void loraSnakeSetTxFlag(void){
  	loraSnakeTransmit = true;
}

static void loraSnakeSetRxFlag(void){
  	loraSnakeReceive = true;
	taskQueue.push(taskQueue.buildTask(TASK_SPACE_COMMS, TASK_SPACE_GOD, palcome_message_mode == 1 ? COMMS_INSTR_RECV_LORA : COMMS_INSTR_RECV));
}

// Private

//public
LoRaSnake::LoRaSnake(void){
  	this->running = false;
  	this->txState = RADIOLIB_ERR_NONE;
  	this->rxState = RADIOLIB_ERR_NONE;
  	loraSnakeTransmit = false;
  	loraSnakeReceive = false;
  	#if LORA_SNAKE_BOARD == LORA_SNAKE_BOARD_T3S3
    		this->_NSS = RADIO_CS_PIN;
    		this->_DIO1 = 33;
    		this->_NRST = 8;
    		this->_BUSY = 34;
    		this->_sck = 5;
    		this->_miso = 3;
    		this->_mosi = 6;
  	#elif LORA_SNAKE_BOARD == LORA_SNAKE_BOARD_TDECK
    		this->_NSS = 9;
    		this->_DIO1 = 45;
    		this->_NRST = 17;
    		this->_BUSY = 13;
    		this->_sck = 40;
    		this->_miso = 38;
    		this->_mosi = 41;
  	#endif
}

LoRaSnake::~LoRaSnake(){

}

void LoRaSnake::denit(void){
	this->sendStop();
	this->listenStop();
	_radio.clearPacketReceivedAction();
	_radio.clearPacketSentAction();
	
}
bool LoRaSnake::init(void){ 
	float freq = 868.0;
  	int err = _radio.begin(freq);

  	if(err == RADIOLIB_ERR_NONE){
    		_radio.setPacketReceivedAction(loraSnakeSetRxFlag);
		int s = _radio.startReceive();

		delay(3000); // Delay may not be required anymore.
  		
		if (s == RADIOLIB_ERR_NONE){
			running = true;
			loraSnakeReceive = false;
  		}
    		return true;
  	}else{
    		Serial.printf("Error no : %d\n", err);
  	}    
  	return false;
}

void LoRaSnake::modeSend(void){
    	_radio.clearPacketReceivedAction();
    	_radio.setPacketSentAction(loraSnakeSetTxFlag);
}

void LoRaSnake::modeRecv(void){
	_radio.clearPacketSentAction();
    	_radio.setPacketReceivedAction(loraSnakeSetRxFlag);
}

bool LoRaSnake::listenStart(void){
  	int s = _radio.startReceive();
  	delay(300);
  	if (s == RADIOLIB_ERR_NONE){
    		running = true;
    		loraSnakeReceive = false;
    		return true;
  	}
	Serial.printf("LISTEN ERROR CODE %d\n", s);
  	return false;
}

int LoRaSnake::readRecv(void){
  	if(!running){
    		loraSnakeReceive = false;
    		this->listenStart();
    		return 0;
  	}else if(loraSnakeReceive){
    		loraSnakeReceive = false;
    		lrsPacket.data_size = _radio.getPacketLength();
    		this->rxState = _radio.readData(lrsPacket.data, lrsPacket.data_size);

    		if(this->rxState == RADIOLIB_ERR_NONE){
      			lrsPacket.rssi = _radio.getRSSI();
      			lrsPacket.snr = _radio.getSNR();
      			lrsPacket.freq_err = _radio.getFrequencyError();
      			return 1;
    		}else if(this->rxState == RADIOLIB_ERR_CRC_MISMATCH){
      			return 2; // malformed packet
    		}else{
      			return 0; // unknown error
    		}
  	}else{
    		return 0;
  	}
}

void LoRaSnake::listenStop(void){
  	_radio.standby();
  	SPI.begin(this->_sck, this->_miso, this->_mosi);
  	running = false;
}

void LoRaSnake::forceSendMode(void){
  	loraSnakeTransmit = true; 
}

void LoRaSnake::sendStart(String v){
  	txState = _radio.startTransmit(v.c_str());
}

void LoRaSnake::sendStart(unsigned char *v, size_t l){
  	txState = _radio.startTransmit(v, l, 0);
}

bool LoRaSnake::send(uint8_t *d, size_t s){
  	if(s >= 256)
    		s= 255;
  	if(running){
    		listenStop();
    		delay(500);
  	}

  	sendStart(d, s);

	/* This efficency variable is  used to 
	 * estimate the amount of time the esp32s3 
	 * chip takes to shift bytes into the lora 
	 * register. Without it, you get data corruption.
	 * */
  	int efficency = (8 *  s) < 333 ? 333 : (8 *  s);
  	delay(efficency);
  
  	if(txState == RADIOLIB_ERR_NONE){
      		sendStop();
      		return true;
    	}else{
      		Serial.printf("Radio Error %d\n", txState);
    	}
    	loraSnakeTransmit = false;
    	sendStop();
    
    	return false;
}

void LoRaSnake::sendStop(void){
  	_radio.finishTransmit();
}


void LoRaSnake::setBandwidth(float v){
	if(v != 7.8   && v != 10.4  && v != 15.6 && 
	   v != 20.8  && v != 31.25 && v != 41.7 && 
	   v != 62.5  && v != 125.0 && v != 250.0
	&& v != 500.0){
		Serial.printf("bandwidth must be one of the following : 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0\n");
		return;
	}

	_radio.setBandwidth(v);
}

void LoRaSnake::setSpreadFactor(int v){
	if(v <= 4 || v >= 13){
		Serial.printf("Spread factor must be between 5 and 12.\n");
		return;
	}
	_radio.setSpreadingFactor(v);
}

void LoRaSnake::setCodingRate(int v){
	if(v <= 4 || v >=9){
		Serial.printf("Coding rate must be between 5 and 8");
		return;
	}
	_radio.setCodingRate(v);
}
				  // 0x44
void LoRaSnake::setSyncWord(int s, int ctrl){
	_radio.setSyncWord(s, ctrl);
}

void LoRaSnake::setPreambleLength(size_t v){
	if(!(v >= 1 && v <= 65535)){
		Serial.printf("Value must be between 1 and 65535\n");
		return;
	}
	_radio.setPreambleLength(v);
}
