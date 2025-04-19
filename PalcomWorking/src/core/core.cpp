#include <Arduino.h>
#include <cstdint>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <SD.h>
#include <Wire.h>
#define TOUCH_MODULES_GT911
#include <TouchLib.h>

#include <defines.h>
#include <src/error/error.h>
#include <src/LoRaSnake/LoRaSnake.class.h>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomObject/PalcomObject.h>
#include <src/PalcomObject/Textarea/Textarea.h>

#include <src/taskQueue/taskQueue.h>

#include <src/core/usb/usb.h>


extern LoRaSnake loraSnake;
extern Usb usb;
extern TaskQueue taskQueue;

extern TFT_eSPI tft;

extern lv_color_t *draw_buf;
extern lv_obj_t *keyboardFocusedObj; 

extern TouchLib *touch;
extern bool touchDected;
extern uint8_t touchAddress;
extern SemaphoreHandle_t xSemaphore;
extern lv_group_t *keyboardGroup;
extern bool sd_card_available;

#include "./core.functions.h"
#include "./core.h"

void GodCore::run_initLora(void){
	if(this->dmaEnabled){
		tft.deInitDMA();
	}
	SPI.end();
    	SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
	bool err = loraSnake.init();
	if(err == false){
		while(1){
			Serial.printf("Failed to initalize LoRa system.\n");
			delay(1000);
		}
	}else{
		Serial.printf("Radio Setup Victorious.\n");
	}
    	SPI.end();
	if(this->dmaEnabled){
    		SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
		tft.deInitDMA();
	}
}
void GodCore::run_denitLoRa(void){
	if(this->dmaEnabled){
		tft.deInitDMA();
	}
	SPI.end();
    	SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
	loraSnake.denit();
}

void GodCore::run_initUsb(void){
	usb.init();
}
void GodCore::run_denitUsb(void){
	usb.denit();
}

void GodCore::run_initWifi(void){}
void GodCore::run_denitWifi(void){}

void GodCore::run_initPins(void){
	this->setupCount++;
	Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
	// Power init
	pinMode(BOARD_POWERON, OUTPUT);
        digitalWrite(BOARD_POWERON, HIGH);
	delay(20);	
	// SPI Init
	pinMode(BOARD_SDCARD_CS, OUTPUT);
	pinMode(RADIO_CS_PIN, OUTPUT);
	pinMode(BOARD_TFT_CS, OUTPUT);
	digitalWrite(BOARD_SDCARD_CS, HIGH);
	digitalWrite(RADIO_CS_PIN, HIGH);
	digitalWrite(BOARD_TFT_CS, HIGH);
	pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
	SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
	delay(20);
	// Misc
	pinMode(BOARD_BOOT_PIN, INPUT_PULLUP);
	pinMode(BOARD_TBOX_G02, INPUT_PULLUP);
	pinMode(BOARD_TBOX_G01, INPUT_PULLUP);
	pinMode(BOARD_TBOX_G04, INPUT_PULLUP);
	pinMode(BOARD_TBOX_G03, INPUT_PULLUP);
	pinMode(42, OUTPUT); // TFT BACKLIGHT
	digitalWrite(42, HIGH); // Force backlight to stay on.
	delay(20);
	// Wakup Touch 
	pinMode(BOARD_TOUCH_INT, OUTPUT);
	digitalWrite(BOARD_TOUCH_INT, HIGH);
	delay(20);
}

void GodCore::run_initLcd(void){
	this->setupCount++;
	tft.begin();
	tft.setRotation( 1 );
	tft.setSwapBytes(false);
	tft.fillScreen(TFT_BLACK);
	tft.initDMA();
	this->dmaEnabled = true;
}
void GodCore::run_initTouch(void){
	this->setupCount++;
	pinMode(BOARD_TOUCH_INT, INPUT);
	delay(20); // Allow time for configuration change.
	scanForTouchDevices(&Wire);
	touch = new TouchLib(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, touchAddress);
	touch->init();
	Wire.beginTransmission(touchAddress);
	touchDected = Wire.endTransmission() == 0;
}

void GodCore::run_initLvgl(void){
	this->setupCount++;
	lv_init();
	lv_tick_set_cb(&callback_calcTick);
	draw_buf = (lv_color_t *)ps_malloc(LVGL_BUFFER_SIZE);
	if(draw_buf == NULL){
		throw CoreException("ESP32Initalizer::lvglInit() - Failed to allowcate draw buffer in PSRAM.", 0x02);
	}
	display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
	if(display == NULL){
		throw CoreException("ESP32Initalizer::lvglInit() - Failed to create Display", 0x01);
	}
	lv_display_set_resolution(display, SCREEN_HOR, SCREEN_VIR);
	lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
	lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf)*LVGL_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
	lv_display_set_flush_cb(display, &disp_flush);

	indev_touchpad = lv_indev_create();
	if(indev_touchpad == NULL){
		throw CoreException("ESP32Initalizer::lvglInit() - Failed to create touchpad input device.", 0x03);
	}
	lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
	lv_indev_set_read_cb(indev_touchpad, &touchpad_read);

	indev_keypad = lv_indev_create();
	if(indev_keypad == NULL){
		while(1){
			Serial.printf("ESP32Initalizer::lvglInit() - Failed to create keypad input device.\n");
			delay(1000);
		}
	}
	lv_indev_set_type(indev_keypad, LV_INDEV_TYPE_KEYPAD);
	lv_indev_set_read_cb(indev_keypad, &keypad_read);
	lv_indev_set_group(indev_keypad, keyboardGroup);
}
void GodCore::run_initSd(void){
	this->setupCount++;
	while(xSemaphoreTake(xSemaphore, 2000*portTICK_PERIOD_MS) != pdTRUE){delay(100);}
	if(dmaEnabled)
		tft.deInitDMA();
	SPI.end();
	SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
	delay(2000);
	if(SD.begin(BOARD_SDCARD_CS)){
		xSemaphoreGive(xSemaphore);
		uint8_t cardType = SD.cardType();
		uint32_t cardSize = SD.cardSize() / (1024 * 1024);
		uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
		uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);

		if(cardType == CARD_NONE){
			SPI.end();
			SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
			tft.initDMA();
			Serial.println("No SD_MMC card attached");
			return;
		}else{
			Serial.print("SD_MMC Card Type: ");
			if(cardType == CARD_MMC){
				Serial.println("MMC");
			}else if(cardType == CARD_SD) {
				Serial.println("SDSC");
			}else if(cardType == CARD_SDHC) {
				Serial.println("SDHC");
			}else{
				Serial.println("UNKNOWN");
			}
			Serial.printf("SD Card Size: %lu MB\n", cardSize);
			Serial.printf("Total space: %lu MB\n",  cardTotal);
			Serial.printf("Used space: %lu MB\n",   cardUsed);
			SPI.end();
			if(dmaEnabled){
				SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
				tft.initDMA();
			}
			sd_card_available = true;
			return;
		}
	}else{
		Serial.printf("Failed to start SD card.\n");
	}
	xSemaphoreGive(xSemaphore);
	SPI.end();
	SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
	if(dmaEnabled){
		tft.initDMA();
	}
}


GodCore::GodCore(void){
	this->spaceType = TASK_SPACE_GOD;
	this->dmaEnabled = false;
	this->setupCount = 0;
}
GodCore::~GodCore(){}

bool GodCore::pop(void){
        struct task_queue_task t = taskQueue.pop(this->spaceType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

bool GodCore::ready(void){
	return (setupCount == 5);
}
void GodCore::process(void){
	if(this->pop()){
	switch(this->task.instruction){
		case GOD_INSTR_PIN_INIT:{
			delay(200);
			this->run_initPins();
		}
		break;		
		case GOD_INSTR_LCD_INIT:{
			delay(200);
			this->run_initLcd();
		}
		break;
		case GOD_INSTR_TOUCH_INIT:{
			delay(200);
			this->run_initTouch();
		}
		break;
		case GOD_INSTR_LVGL_INIT:{
			delay(200);
			this->run_initLvgl();
		}
		break;
		case GOD_INSTR_SD_INIT:{
			delay(200);
			this->run_initSd();
		}
		break;
	}
	}
}

void GodCore::initLora(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_LORA_INIT
		)
	);
}
void GodCore::denitLoRa(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_LORA_DENIT
		)
	);

}
		
void GodCore::initUsb(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_USB_INIT
		)
	);

}
void GodCore::denitUsb(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_USB_DENIT
		)
	);

}

void GodCore::initWifi(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_WIFI_INIT
		)
	);

}
void GodCore::denitWifi(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_WIFI_DENIT
		)
	);

}

void GodCore::initPins(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_PIN_INIT
		)
	);
}
void GodCore::initLcd(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_LCD_INIT
		)
	);
}
void GodCore::initTouch(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_TOUCH_INIT
		)
	);
}
void GodCore::initLvgl(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_LVGL_INIT
		)
	);
}
void GodCore::initSd(void){
	taskQueue.push(
		taskQueue.buildTask(
			TASK_SPACE_GOD, 
			TASK_SPACE_GOD, 
			GOD_INSTR_SD_INIT
		)
	);
}
