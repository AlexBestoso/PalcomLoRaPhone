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
//SPIClass hhhpi(HSPI);
//SPISettings hhhpiSettings(40000000, MSBFIRST, SPI_MODE0);
#include <TFT_eSPI.h>
#include <RadioLib.h>
#include <lvgl.h>
#include <string>

#include "USB.h"

#if !ARDUINO_USB_CDC_ON_BOOT
USBCDC USBSerial;
#endif



#include <SD.h>

#define TOUCH_MODULES_GT911
#include "TouchLib.h"
#include <AceButton.h>
using namespace ace_button;
using namespace std;

#include "./src/spiDebug/spiDebug.class.h"
SpiDebug spi2_a, spi2_b, spi3_a, spi3_b;

#include "utilities.h"
#include "./core/structs/structs.h"
#include "./core/tools/PalcomFS.h"
#include "./core/error/error.h"

#include "./src/LoRaSnake/LoRaSnake.class.h"
LoRaSnake loraSnake;

#include "./core/initalizer/initalizer.h"



/*
 * These may be refactored to be less dependant on each other.
 */


#include "./core/event/event.h"
#include "./core/tools/colors.tool.h"
#include "./core/partition/partition.h"
#include "./core/styles/styles.h"

#include "./core/objects/objects.h"
#include "./core/screens/screens.h"

static void GraphicsTask(void *parm);
static void CommsTask(void *parm);
static void StorageTask(void *parm);



bool getInput(void){
  
  if(userBufferIdx >= 0 && userBufferIdx < USER_BUF_SIZE){
    int v = keypad_get_key();
    if(v == 0x00)
      return false;
    userBuffer[userBufferIdx] = v;
    userBufferIdx++;
    userBufferSize = userBufferIdx;
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

PalcomDebugScreen pds;

static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_STARTED_EVENT: Serial.println("USB PLUGGED"); break;
      case ARDUINO_USB_STOPPED_EVENT: Serial.println("USB UNPLUGGED"); break;
      case ARDUINO_USB_SUSPEND_EVENT: Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", data->suspend.remote_wakeup_en); break;
      case ARDUINO_USB_RESUME_EVENT:  Serial.println("USB RESUMED"); break;

      default: break;
    }
  } else if (event_base == ARDUINO_USB_CDC_EVENTS) {
    arduino_usb_cdc_event_data_t *data = (arduino_usb_cdc_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_CDC_CONNECTED_EVENT:    Serial.println("CDC CONNECTED"); break;
      case ARDUINO_USB_CDC_DISCONNECTED_EVENT: Serial.println("CDC DISCONNECTED"); break;
      case ARDUINO_USB_CDC_LINE_STATE_EVENT:   Serial.printf("CDC LINE STATE: dtr: %u, rts: %u\n", data->line_state.dtr, data->line_state.rts); break;
      case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        Serial.printf(
          "CDC LINE CODING: bit_rate: %lu, data_bits: %u, stop_bits: %u, parity: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits,
          data->line_coding.stop_bits, data->line_coding.parity
        );
        break;
      case ARDUINO_USB_CDC_RX_EVENT:
        Serial.printf("Processing Serial Command...\n");
        {
            uint8_t buffer[data->rx.len] = {0};
            size_t len = USBSerial.read(buffer, data->rx.len);
            if(len <= 4){
              return;
            }
            String test = "";
            for(int i=0; i<len; i++)
              test+=(char)buffer[i];
            Serial.printf("USB : %s\n", test.c_str());
            if(test == "aaaaa" || test == "aaaaa\n")
            pds.test = true;
        }
        break;
      case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT: Serial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes); break;

      default: break;
    }
  }
}



bool autoComp(int spiInf, bool refresh=true){
  if(spiInf != 2 || spiInf != 3)
    spiInf = 2;

  if(refresh && spiInf == 2)
    spi2_b.refresh();
  else if(refresh){
    spi3_b.refresh();
  }
  

  return spiInf == 2 ? spi2_a.compAll(spi2_b, true) : spi3_a.compAll(spi3_b, true);
}


bool dbgc3(){
  return autoComp(3, true);
}
bool coreOne = false;
bool coreTwo = false;
String loraListenRes = "";
void setup(void){
  //Serial.begin(115200);
  Serial.begin(9600);
  initer.pinInit();
  delay(2000);
  Serial.printf("[Fresh Reboot]\n");
  delay(2000);
  

  spi2_a.setSpiMode(2);
  spi2_b.setSpiMode(2);
  spi3_a.setSpiMode(3);
  spi3_b.setSpiMode(3);

  spi2_a.init();
  spi2_b.init();
  spi3_a.init();
  spi3_b.init();
  Serial.printf("Starting Semaphores\n");
  try{
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);

    if(xSemaphore == NULL || xSemaphore == nullptr)
      throw CoreException("Failed to create Semaphore.", ERR_TASK_SEMAPHORE);

    if(xTaskCreatePinnedToCore(GraphicsTask, "graphics", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create Graphics Task", ERR_TASK_CREATE);
    }

    if(xTaskCreatePinnedToCore(CommsTask, "communication", 4096*2, NULL, 5, NULL, 0) != pdPASS){
      throw CoreException("Failed to create Comms Task", ERR_TASK_CREATE);
    }

    if(xTaskCreatePinnedToCore(UserInputTask, "user_input", 4096*2, NULL, 5, NULL, 1) != pdPASS){
      throw CoreException("Failed to create User Input Task", ERR_TASK_CREATE);
    }


    //SPI.end();
    //SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);

  }catch(CoreException &ce){
      ce.halt();
  }
  



   //xTaskCreatePinnedToCore(noggerTask, "gui", 4096*2, NULL, 1, NULL, 1);
  

 
  //delay(1000);

  //initer.setupRadio();

  
    //initer.setupSD();  

  

 
  
  while(!coreOne || !coreTwo){delay(100);}
  Serial.printf("initlaizing USB interface.\n");
  USBSerial.onEvent(usbEventCallback);
  USB.onEvent(usbEventCallback);

  USBSerial.begin();
  USB.begin();
  /*Serial.printf("USB interface Successfully set up\n");
  bool err = loraSnake.listenStart();
  if(err){
    Serial.printf("LoRa System Listening for messages.\n");
  }else{
    Serial.printf("LoRa System FAILED to listen for messages.\n");
  }*/
  Serial.printf("At the end of init function\n");
}

static void GraphicsTask(void *parm){
  while(!coreTwo){delay(1000);}
  //initer.semaphoreInit();

  initer.aceButtonInit();
  
  initer.lcdInit();
  initer.touchscreenInit();
  

   initer.lvglInit();

  coreOne = true;
  bool ddbg = false;
  while (1) {
    
   pds.run();
    if(pds.test == true){
      //pds.test = true;
      if(ddbg == false)
        pds.reset();
      ddbg = true;
    }
    if(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) == pdTRUE){
      /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
      vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Gui Task\n", xPortGetCoreID());
      //pds.run();
      xSemaphoreGive(xSemaphore);
    }else{
      Serial.printf("%d semaphore failed\n", xPortGetCoreID());
    }
    delay(500);
   }
}
static void CommsTask(void *parm){
  initer.setupRadio();

  
  /*if(loraSnake.listenStart()){
    Serial.printf("LoRa System Listening for messages.\n");
  }else{
    Serial.printf("LoRa System FAILED to listen for messages.\n");
  }*/
  /*  initer.pinInit();
    spi2_a.refresh();
    spi2_b.refresh();

  while(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) != pdTRUE){delay(100);}

    Serial.printf("Starting lora snake.\n");
    if(!loraSnake.init()){
      delay(2000);
      if(spi2_a.compAll(spi2_b, true))
        Serial.printf("SPI2 : No Change\n");
      else
        Serial.printf("SPI2 : Change Detected\n");
      xSemaphoreGive(xSemaphore);
      coreTwo = true;
      while(1){
              Serial.printf("Failed to initalize LoRa system.\n");

              delay(1000);
      }
    }
      if(spi2_a.compAll(spi2_b, true))
        Serial.printf("SPI2 : No Change\n");
      else
        Serial.printf("SPI2 : Change Detected\n");

    xSemaphoreGive(xSemaphore);
    coreTwo = true;
  */
  coreTwo = true;

  while(1){
    if(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) == pdTRUE){
      vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Comms Task\n", xPortGetCoreID());
      xSemaphoreGive(xSemaphore);
    }else{
      Serial.printf("%d semaphore failed\n", xPortGetCoreID());
    }
    delay(2000);
  }
}
static void UserInputTask(void *parm){
  Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
  initer.touchscreenInit();
  
  
  kbDected = checkKb();
  if(kbDected){
    Serial.printf("Keyboard initalized...\n");
  }else{
    Serial.printf("No Keyboard found.\n");
  }

  while(1){
  getInput();
  if(userBufferSize > 0){
    Serial.printf("input : ");
    for(int i=0;  i<userBufferSize; i++)
      Serial.printf("%c", userBuffer[i]);
    Serial.printf("\n");
  }
  if(processInput()){
    clearInput();
  }
    /*if(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) == pdTRUE){
      vTaskDelay(pdMS_TO_TICKS(10));
      //Serial.printf("%d Storage Task\n", xPortGetCoreID());
      xSemaphoreGive(xSemaphore);
    }else{
      Serial.printf("%d semaphore failed\n", xPortGetCoreID());
    }*/
    delay(20);
  }
}

void loop(){
      //pds.run();
 /* getInput();
  if(userBufferSize > 0){
    Serial.printf("input : ");
    for(int i=0;  i<userBufferSize; i++)
      Serial.printf("%c", userBuffer[i]);
    Serial.printf("\n");
  }
  if(processInput()){
    if(!loraSnake.send(userBuffer, userBufferSize-1)){
      Serial.printf("Failed to send message.\n");
    }else{
      Serial.printf("Sent : [%d] \n", userBufferSize-1);
    }
    loraSnake.listenStart();
    
    clearInput();
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
  }*/

  delay(1000);
}