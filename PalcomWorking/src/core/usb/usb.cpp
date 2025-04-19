#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <src/taskQueue/taskQueue.h>
#include "USB.h"

#if !ARDUINO_USB_CDC_ON_BOOT
USBCDC USBSerial;
#endif

#include "./usb.h"

extern SemaphoreHandle_t xSemaphore;
extern TaskQueue taskQueue;
extern bool sd_card_available;
extern char displayed_messages[10][257];
extern int displayed_page;

static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
 	if(event_base == ARDUINO_USB_EVENTS){
    		arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    		switch (event_id) {
      			case ARDUINO_USB_STARTED_EVENT: 
				Serial.println("USB PLUGGED"); 
			break;
      			case ARDUINO_USB_STOPPED_EVENT: 
				Serial.println("USB UNPLUGGED"); 
			break;
      			case ARDUINO_USB_SUSPEND_EVENT: 
				Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n", 
					data->suspend.remote_wakeup_en); 
			break;
      			case ARDUINO_USB_RESUME_EVENT:
				Serial.println("USB RESUMED");
			break;
      			default: break;
    		}
  	}else if (event_base == ARDUINO_USB_CDC_EVENTS) {
    		arduino_usb_cdc_event_data_t *data = (arduino_usb_cdc_event_data_t *)event_data;
    		switch (event_id) {
      		case ARDUINO_USB_CDC_CONNECTED_EVENT:
			Serial.println("CDC CONNECTED");
		break;
      		case ARDUINO_USB_CDC_DISCONNECTED_EVENT:
			Serial.println("CDC DISCONNECTED"); 
		break;
      		case ARDUINO_USB_CDC_LINE_STATE_EVENT:   
			Serial.printf("CDC LINE STATE: dtr: %u, rts: %u\n", 
				data->line_state.dtr, data->line_state.rts); 
		break;
      		case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        		Serial.printf(
          		"CDC LINE CODING: bit_rate: %lu, data_bits: %u, stop_bits: %u, parity: %u\n", 
			data->line_coding.bit_rate, data->line_coding.data_bits,
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
        		}
        	break;
      		case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT:
			Serial.printf("CDC RX Overflow of %d bytes", 
				data->rx_overflow.dropped_bytes); 
		break;

      		default: break;
    		}
  	}
}



//Private
bool Usb::pop(void){
	struct task_queue_task t = taskQueue.pop(this->spaceType);
	if(!t.active)
		return false;
	this->task = t;
	return true;
}


//Public
Usb::Usb(void){
	this->spaceType = TASK_SPACE_USB;
}
Usb::~Usb(){}


void Usb::init(void){
	USBSerial.onEvent(usbEventCallback);
    	USB.onEvent(usbEventCallback);

    	USBSerial.begin();
    	USB.begin();
}

void Usb::denit(void){
	USBSerial.end();
}
bool Usb::fetchTask(void){
	return this->pop();
}

bool Usb::runTask(void){
	switch(this->task.instruction){
		case USB_INSTR_SETUP:
			return true;
			break;
		default:
			return false;
	}
	return false;
}
