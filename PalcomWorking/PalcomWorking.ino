/**
 * @file      PalcomWorking.ino
 * @author    Morning Star
 * @license   Morning Star Usage License
 * @note      Arduino Setting
 *            Tools ->
 *                  Board:"ESP32S3 Dev Module"
 *                  USB CDC On Boot:"Enable"
 *                  USB DFU On Boot:"Disable"
 *                  Flash Size : "16MB(128Mb)"
 *                  Flash Mode"QIO 80MHz
 *                  Partition Scheme:"16M Flash(3M APP/9.9MB FATFS)"
 *                  PSRAM:"OPI PSRAM"
 *                  Upload Mode:"UART0/Hardware CDC"
 *                  USB Mode:"Hardware CDC and JTAG"
 *
 *      https://lvgl.io/tools/imageconverter
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>
#include <lvgl.h>
#include <string>

#if LV_USE_TFT_ESPI
  #include <TFT_eSPI.h>
#endif

#include <SD.h>

#define TOUCH_MODULES_GT911
#include "TouchLib.h"
#include "utilities.h"
#include <AceButton.h>

#include "./LoRaSnake.class.h"

using namespace ace_button;

void setup(void){
  Serial.begin(115200);
  if(!loraSnake.init()){
    while(1){
      Serial.printf("Failed to initalize LoRa system.\n");
      delay(1000);
    }
  }

  loraSnake.listenStart();
}


void loop(){
  if(Serial.available() != 0){
    String v = Serial.readString();
    if(!loraSnake.send((uint8_t *)v.c_str(), v.length())){
      Serial.printf("Failed to send message.\n");
    }else{
      Serial.printf("Sent : [%d] %s\n", v.length(), v.c_str());
    }
    loraSnake.listenStart();
  }
  
  if(loraSnake.readRecv() && loraSnake.lrsPacket.data_size > 0){
    Serial.printf("Received the message : [%d] ", 
                  loraSnake.lrsPacket.data_size);

    for(int i=0; i<loraSnake.lrsPacket.data_size; i++){
      Serial.printf("%c", loraSnake.lrsPacket.data[i]);
    }
    Serial.printf("\n");
    for(int i=0; i<256; i++)
      loraSnake.lrsPacket.data[i] = 0;
    loraSnake.lrsPacket.data_size = 0;
  }
  delay(1000);
}



























