#include "./LoRaSnake.class.h"

extern bool loraSnakeTransmit = false;
extern bool loraSnakeReceive = false;
extern SPIClass ssspi(HSPI);
extern SPISettings ssspiSettings(2000000, MSBFIRST, SPI_MODE0);
extern SX1262 _radio = new Module(9, 45, 17, 13);

static void loraSnakeSetTxFlag(void){
  loraSnakeTransmit = true;
}
static void loraSnakeSetRxFlag(void){
  loraSnakeReceive = true;
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

bool LoRaSnake::init(void){ 
  float freq=868.0;
  //this->_radio = new Module(9, 45, 17, 13);
  //digitalWrite(BOARD_SDCARD_CS, HIGH);
  //digitalWrite(BOARD_TFT_CS, HIGH);
  // digitalWrite(RADIO_CS_PIN, HIGH);  
  //SPI.end();
  
  ////SPI.begin(_sck, _miso, _mosi);
  //ssspi.end();
  //ssspi.begin(_sck, _miso, _mosi);
  int err = 0;
  SPI.begin(this->_sck, this->_miso, this->_mosi);
  err = _radio.begin(freq);
  if(err == RADIOLIB_ERR_NONE){
    _radio.setPacketSentAction(loraSnakeSetTxFlag);
    _radio.setPacketReceivedAction(loraSnakeSetRxFlag);

	//_radio.sleep(true)
    //_radio.standby();
	int s = _radio.startReceive();
	delay(3000);
  	if (s == RADIOLIB_ERR_NONE){
		Serial.printf("LoRa Listening...\n");
  	}
    return true;
  }else{
    Serial.printf("Error no : %d\n", err);
  }    
  return false;
}

bool LoRaSnake::listenStart(void){
	delay(1000);
  int s = _radio.startReceive();
  delay(3000);
  if (s == RADIOLIB_ERR_NONE){
    running = true;
    loraSnakeReceive = false;
    return true;
  }
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
      //Serial.printf("Malformed packet received.");
      return 2; // malformed packet
    }else{
      Serial.printf("Unknown Error");
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
  txState = _radio.startTransmit(v, l);
}

bool LoRaSnake::send(uint8_t *d, size_t s){
  if(running){
    listenStop();
    delay(500);
  }
  if(s >= 256)
    s= 255;
  sendStart(d, s);
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
