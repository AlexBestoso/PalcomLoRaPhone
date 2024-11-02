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

#include "USB.h"

#if !ARDUINO_USB_CDC_ON_BOOT
USBCDC USBSerial;
#endif

#if LV_USE_TFT_ESPI
  #include <TFT_eSPI.h>
#endif

#include <SD.h>

#define TOUCH_MODULES_GT911
#include "TouchLib.h"
#include <AceButton.h>
using namespace ace_button;



#include "utilities.h"

#include "./core/initalizer/initalizer.h"
#include "./LoRaSnake.class.h"



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
         /*   if(!loraSnake.send(buffer, len)){
              Serial.printf("Failed to send message.\n");
            }else{
              Serial.printf("Sent : [%d]", len);
              for(int i=0; i<len; i++)
                Serial.printf("%c", buffer[i]);
              Serial.printf("\n");
            }
*/
            //loraSnake.listenStart();
        }
        break;
      case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT: Serial.printf("CDC RX Overflow of %d bytes", data->rx_overflow.dropped_bytes); break;

      default: break;
    }
  }
}

void setup(void){
  Serial.begin(115200);
  pinMode(44, INPUT);
  digitalWrite(44, HIGH);
  if(digitalRead(44) == LOW ){
    while(1){
        Serial.printf("ERROR CATCHER...: %d\n", SPI_PORT);
        delay(1000);

    }
  }
  
  initer.pinInit();
  initer.setupSD();
  initer.aceButtonInit();
  initer.semaphoreInit();
  initer.lcdInit();
  initer.touchscreenInit();
  kbDected = checkKb();
  if(kbDected){
    Serial.printf("Keyboard initalized...\n");
  }else{
   // while(1){
   //   delay(1000);
      Serial.printf("Failed to detect keyboard.\n");
   // }
  }
  initer.lvglInit();

  
   if(!loraSnake.init()){
    while(1){
      Serial.printf("Failed to initalize LoRa system.\n");
      delay(1000);
    }
  }

  USBSerial.onEvent(usbEventCallback);
  USB.onEvent(usbEventCallback);

  USBSerial.begin();
  USB.begin();

  loraSnake.listenStart();
}


void loop(){
  keypad_get_key();
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