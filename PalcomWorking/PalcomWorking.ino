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
 *                  Upload Mode:"USB-OTG CDC (tiny usb)"
 *                  USB Mode:"USB-OTG (tiny usb)"
 *
 *      https://lvgl.io/tools/imageconverter
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <RadioLib.h>
#include <lvgl.h>
#include <string>
#include <cstdint>

#include <mbedtls/md.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/bignum.h>
#include <mbedtls/x509.h>
#include <mbedtls/rsa.h>
#include <mbedtls/aes.h>

#include "USB.h"

#if !ARDUINO_USB_CDC_ON_BOOT
USBCDC USBSerial;
#endif



#include <SD.h>

#define TOUCH_MODULES_GT911
#include <TouchLib.h>
#include <AceButton.h>
using namespace ace_button;
using namespace std;

#include "utilities.h"
#include "./core/structs/structs.h"
#include "./core/tools/PalcomFS.h"
#include <src/error/error.h>

#include <src/taskQueue/taskQueue.h>
TaskQueue taskQueue;

#include <src/LoRaSnake/LoRaSnake.class.h>
LoRaSnake loraSnake;
/*
#include <src/cryptography/cryptography.h>
Cryptography cryptography;


#include <src/wifi/wifi.h>
PalcomWifi wifi;
#include <src/PalcomSettings/settings.h>
palcom_partition_t COMMS_SETTINGS;

#include <src/PalcomEvent/PalcomEvent.h>
//#include "./src/PalcomColors/PalcomColors.h"
//#include "./core/partition/partition.h"
#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>
#include "./core/styles/styles.h"

#include <src/PalcomObject/PalcomObject.h>
#include <src/PalcomObject/Label/Label.h>
#include <src/PalcomObject/Button/Button.h>
#include <src/PalcomObject/Image/Image.h>
#include <src/PalcomObject/Textarea/Textarea.h>
#include <src/PalcomObject/Line/Line.h>
#include <src/PalcomObject/Triangle/Triangle.h>
#include <src/PalcomObject/Dropdown/Dropdown.h>
#include <src/PalcomObject/spinner/spinner.h>
#include <src/PalcomObject/tabview/tabview.h>

#include "./core/initalizer/initalizer.h"

#include <src/PalcomScreen/PalcomScreen.h>
#include <src/PalcomScreen/DebugScreen/DebugScreen.h>
#include <src/PalcomScreen/setMsgMode/setMsgMode.h>
//#include "./core/objects/objects.h"
//#include "./core/screens/screens.h"

#include <src/core/core.h>
GodCore core;

#include <src/core/storage/storage.h>
Storage storage;

#include <src/core/graphics/graphics.h>
Graphics graphics;

#include <src/core/comms/comms.h>
Comms comms;

#include <src/core/_usb/_usb.h>
Usb usb;

static void GraphicsTask(void *parm);
static void CommsTask(void *parm);
static void StorageTask(void *parm);
static void UserInputTask(void *parm);

/*
bool getInput(void){
  
  if(userBufferIdx >= 0 && userBufferIdx < USER_BUF_SIZE){
    int v = keypad_get_key();
   
    if(v == 0x00){
      return false;
    }else if (v == 8){
      userBuffer[userBufferIdx] = 0x0;
      userBufferIdx = userBufferIdx == 0 ? 0 : userBufferIdx-1;
      userBufferSize = userBufferIdx;
    }else{
      userBuffer[userBufferIdx] = v;
      userBufferIdx++;
      userBufferSize = userBufferIdx;
    }
    return true;
  }
  return false;
}

void clearInput(void){
  for(int i=0; i<USER_BUF_SIZE; i++)
    userBuffer[i] = 0;
  userBufferSize = 0;
  userBufferIdx = 0;
}
bool processInput(void){
  if(userBufferSize <= 0)
    return false;
  if(userBuffer[userBufferSize-1] == 0xd)
    return true;
  return false;
}
*/
/*
bool coreOne = false;
bool coreTwo = false;
bool coreThree = false;

String loraListenRes = "";


static void GraphicsTask(void *parm){
  while(!coreTwo){delay(1000);}
  //initer.semaphoreInit();

  
  

  
  coreOne = true;
  
  while (1) {
    //if(xSemaphoreTake(xSemaphore, 0) == pdTRUE){

      
      
      //vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Comms Task\n", xPortGetCoreID());
      //xSemaphoreGive(xSemaphore);
   // }else{
    //  Serial.printf("%d Xsemaphore failed\n", xPortGetCoreID());
    //}
  }
}

static void StorageTask(void *parm){  
  Serial.printf("Setting up Storage\n");
  /*if(!initer.setupSD()){
    while(1){
        Serial.printf("SD FAILURE.\n");
        delay(2000);
    }
  }*/
  /*coreThree = true;
  while(1){
      if(storage.fetchTask()){
        storage.runTask();
      }
    delay(150);
  }
}

static void CommsTask(void *parm){
  
  
  coreTwo = true;

  while(1){
    //if(comms.fetchTask()){
    //  comms.runTask();
    //}
   // if(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) == pdTRUE){
      
     /*
      if(loraSnake.readRecv() && loraSnake.lrsPacket.data_size > 0){
        Serial.printf("Received the message : [%d] ", loraSnake.lrsPacket.data_size);
      }*/
      //vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Comms Task\n", xPortGetCoreID());
     /* xSemaphoreGive(xSemaphore);
    }else{
      Serial.printf("%d Xsemaphore failed\n", xPortGetCoreID());
    }*/
    /*delay(2000);
  }
}

static void UserInputTask(void *parm){
  Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
  //initer.touchscreenInit();
  
  
  kbDected = checkKb();
  if(kbDected){
    Serial.printf("Keyboard initalized...\n");
  }else{
    Serial.printf("No Keyboard found.\n");
  }
  
  while(1){
  /*getInput();
  /*if(userBufferSize > 0){
    if(keyboardFocusedObj != NULL){
      Serial.printf("input : ");
      for(int i=0;  i<userBufferSize; i++)
        Serial.printf("%c", userBuffer[i]);
      Serial.printf("\n");
      PalcomTextarea textarea;
      textarea.setObject(keyboardFocusedObj);
      textarea.setText((const char *)userBuffer);
    }
  }
  if(processInput()){
    clearInput();
  }*/
    
    
    /*if(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) == pdTRUE){
      vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Storage Task\n", xPortGetCoreID());
      xSemaphoreGive(xSemaphore);
    }else{
      Serial.printf("%d semaphore failed\n", xPortGetCoreID());
    }*/
    /*delay(20);
  }
}
*/
void setup(void){
  Serial.begin(115200);
  delay(2000);
  Serial.printf("[Palcoms 2.x.y Alpha]\n");
  
  /*try{
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);

    Serial.printf("Jaun Teat\n");
    core.initPins();
while(1);
    Serial.printf("Init pins good\n");
    core.initLcd();
    Serial.printf("Init Lcd goos\n");
    core.initTouch();
    Serial.printf("Touch Good :D\n");
    core.initLvgl();
    Serial.printf("Lvgl Good\n");
    core.initSd();
    Serial.printf("SD good\n");

    int juan = 1;
    while(juan) delay(300);

    //initer.pinInit();
    //initer.lcdInit();

    //tft.deInitDMA();
    //SPI.end();
    //SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
    //  initer.setupRadio();
    //SPI.end();
    //SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
    //tft.initDMA();

    //initer.aceButtonInit();
    //initer.touchscreenInit();
    

    //initer.lvglInit();

    //sd_card_available = initer.setupSD();

    taskQueue.push(taskQueue.buildTask(TASK_SPACE_STORAGE, TASK_SPACE_GOD, STORAGE_INSTR_REFRESH_MSG));
    taskQueue.push(taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GOD, GRAPHICS_INSTR_SETUP));

   /* if(xSemaphore == NULL || xSemaphore == nullptr)
      throw CoreException("Failed to create Semaphore.", ERR_TASK_SEMAPHORE);

    if(xTaskCreatePinnedToCore(GraphicsTask, "graphics", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create Graphics Task", ERR_TASK_CREATE);
    }

    if(xTaskCreatePinnedToCore(CommsTask, "communication", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create Comms Task", ERR_TASK_CREATE);
    }

    if(xTaskCreatePinnedToCore(UserInputTask, "user", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create User Input Task", ERR_TASK_CREATE);
    }

    if(xTaskCreatePinnedToCore(StorageTask, "storage", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create User Task", ERR_TASK_CREATE);
    }*/

    //while(!coreOne || !coreTwo || !coreThree){delay(100);}
    /*
    usb.init();
    //comms.init(&cryptography, (unsigned char*)CORE_ROUTING_KEY, CORE_ROUTING_KEY_SIZE);
    
    //taskQueue.push(taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GOD, GRAPHICS_INSTR_SETUP));
 /* }catch(CoreException &ce){
      ce.halt();
  }*/
}

void loop(){
      Serial.printf("IslammaJew\n");
      /*core.process();
      if(core.ready()){
       
        if(graphics.fetchTask()){
          graphics.runTask();
          graphics.exec(true);
        }else{
          graphics.exec(false);
        }
        lv_tick_inc(5);
      }
      
      if(comms.fetchTask()){
        tft.deInitDMA();
        SPI.end();
        SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);

        Serial.printf("Running Comms Task\n");
        comms.runTask();

        SPI.end();
        SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
        tft.initDMA();
      }

      if(storage.fetchTask()){
        tft.deInitDMA();
        SPI.end();
        SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);

        Serial.printf("Running Storage Task\n");
        storage.runTask();

        SPI.end();
        SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
        tft.initDMA();
      }*/
  delay(50);
}