#define LVGL_BUFFER_SIZE    (TFT_WIDTH * TFT_HEIGHT ) * sizeof(lv_color_t)
lv_color_t *draw_buf = NULL;

int Setup_setupControl = 0;
int Sleep_interactionCtx = 0;
int Sleep_maxBrightness = 256;
int Sleep_brightness = 0;
float Sleep_timer = millis();


static void Setup_setRxFlag(void){
      		rxFlag = true;
}

static void Setup_setTxFlag(void){
        // we got a packet, set the flag
        txFlag = true;
        transmissionFlag = false;
}

class ESP32Initalizer{
	private:
		lv_timer_t *timer = NULL;
		lv_display_t *display = NULL;
		lv_indev_t *indev_touchpad = NULL;
		lv_indev_t *indev_keypad = NULL;
		lv_indev_t *indev_mouse = NULL;
		lv_obj_t *cursorImg = NULL;
		/*
		 * Private functions
		 * */
		void powerInit(void){
			pinMode(BOARD_POWERON, OUTPUT);
                        digitalWrite(BOARD_POWERON, HIGH);
		}
		void spiInit(void){
			pinMode(BOARD_SDCARD_CS, OUTPUT);
                        pinMode(RADIO_CS_PIN, OUTPUT);
                        pinMode(BOARD_TFT_CS, OUTPUT);

                        digitalWrite(BOARD_SDCARD_CS, HIGH);
                        digitalWrite(RADIO_CS_PIN, HIGH);
                        digitalWrite(BOARD_TFT_CS, HIGH);
			
			pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
			SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
		}

		void miscInit(void){
			pinMode(BOARD_BOOT_PIN, INPUT_PULLUP);
                        pinMode(BOARD_TBOX_G02, INPUT_PULLUP);
                        pinMode(BOARD_TBOX_G01, INPUT_PULLUP);
                        pinMode(BOARD_TBOX_G04, INPUT_PULLUP);
                        pinMode(BOARD_TBOX_G03, INPUT_PULLUP);
		}

		void wakupTouchInit(void){
			pinMode(BOARD_TOUCH_INT, OUTPUT);
                        digitalWrite(BOARD_TOUCH_INT, HIGH);
		}
		void scanForTouchDevices(TwoWire *w){
                        uint8_t err, addr;
                        int nDevices = 0;
                        uint32_t start = 0;
                        for(addr = 1; addr < 127; addr++){
                                start = millis();
                                w->beginTransmission(addr); delay(2);
                                err = w->endTransmission();
                                if(err == 0){
                                        nDevices++;
                                        Serial.print("I2C device found at address 0x");
                                        if(addr < 16){
                                                Serial.print("0");
                                        }
                                        Serial.print(addr, HEX);
                                        Serial.println(" !");

                                        if(addr == GT911_SLAVE_ADDRESS2){
                                                touchAddress = GT911_SLAVE_ADDRESS2;
                                                Serial.println("Find GT911 Drv Slave address: 0x14");
                                        }else if(addr == GT911_SLAVE_ADDRESS1){
                                                touchAddress = GT911_SLAVE_ADDRESS1;
                                                Serial.println("Find GT911 Drv Slave address: 0x5D");
                                        }
                                }else if(err == 4){
                                        Serial.print("Unknow error at address 0x");
                                        if(addr < 16){
                                                Serial.print("0");
                                        }
                                        Serial.println(addr, HEX);
                                }
                        }
                        if(nDevices == 0)
                                Serial.println("No I2C devices found\n");
                }

		/*
		 * Static Functions
		 * */


		static void callback_acebutton(AceButton * /* button */, uint8_t eventType, uint8_t /* buttonState */){
                        switch(eventType){
                                case AceButton::kEventClicked:
                                        clicked = true;
                                        break;
                                case AceButton::kEventLongPressed:
                                        //If you need other peripherals to maintain power, please set the IO port to hold
                                        // gpio_hold_en((gpio_num_t)BOARD_POWERON);
                                        // gpio_deep_sleep_hold_en();
                                        // When sleeping, set the touch and display screen to sleep, and all other peripherals will be powered off
                                        pinMode(BOARD_TOUCH_INT, OUTPUT);
                                        digitalWrite(BOARD_TOUCH_INT, LOW); //Before touch to set sleep, it is necessary to set INT to LOW
                                        touch->enableSleep();        //set touchpad enter sleep mode
                                        tft.writecommand(0x10);     //set disaplay enter sleep mode
                                        delay(2000);
                                        esp_sleep_enable_ext1_wakeup(1ull << BOARD_BOOT_PIN, ESP_EXT1_WAKEUP_ALL_LOW);
                                        esp_deep_sleep_start();
                                        //Deep sleep consumes approximately 240uA of current
                                        break;
                        }
                }

		static uint32_t callback_calcTick(void){
			lv_tick_inc(5);
			return esp_timer_get_time() / 1000;
		}

		static void disp_flush( lv_display_t *disp, const lv_area_t *area, unsigned char *color_p ){
                        uint32_t w = ( area->x2 - area->x1 + 1 );
                        uint32_t h = ( area->y2 - area->y1 + 1 );
			uint32_t s = w*h;

                        if(xSemaphore == NULL){
                                Serial.printf("Semaphore is null\n");
                                return;
                        }

                        if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ){
				Serial.printf("Writing %ld bytes to screen.\n", s);
				lv_draw_sw_rgb565_swap((void *)color_p, s);

                                tft.startWrite();
                                tft.setAddrWindow( area->x1, area->y1, w, h );
                                //tft.pushColors( ( uint16_t * )&color_p, s, false );
				tft.pushPixels((void *)color_p, w*h);
                                tft.endWrite();
                                lv_disp_flush_ready( disp );
                                xSemaphoreGive( xSemaphore );
                        }
                }

		static void touchpad_read( lv_indev_t *indev_driver, lv_indev_data_t *data ){
                        uint8_t rotation = tft.getRotation();
                        if(!touch->read()){
                                data->state = LV_INDEV_STATE_REL;
                                return;
                        }
			Serial.printf("\tTouch Detected!\n");
                        TP_Point t = touch->getPoint(0);
                        switch(rotation){
                                case 1:
                                        data->point.x = t.y;
                                        data->point.y = tft.height() - t.x;
                                        break;
                                case 2:
                                        data->point.x = tft.width() - t.x;
                                        data->point.y = tft.height() - t.y;
                                        break;
                                case 3:
                                        data->point.x = tft.width() - t.y;
                                        data->point.y = t.x;
                                        break;
                                case 0:
                                default:
                                        data->point.x = t.x;
                                        data->point.y = t.y;
                        }
                        Sleep_interactionCtx = 0;
                        Sleep_timer = millis();

                        data->state = LV_INDEV_STATE_PR;
                        return;
                }

		static void keypad_read(lv_indev_t *indev_drv, lv_indev_data_t *data){
                        static uint32_t last_key = 0;
                        uint32_t act_key ;
                        act_key = keypad_get_key();
                        if(act_key != 0){
                                data->state = LV_INDEV_STATE_PR;
                                Sleep_interactionCtx = 0;
                                Sleep_timer = millis();
                                if(act_key == 0x20)
                                        screenLockConditionSpace = true;
                                else
                                        screenLockConditionSpace = false;

                                last_key = act_key;
				Serial.printf("key pressed '%c'\n", act_key);
                        }else{
                                data->state = LV_INDEV_STATE_REL;
                        }
                        data->key = last_key;
                }

		static uint32_t keypad_get_key(void){
                        char key_ch = 0;
                        Wire.requestFrom(0x55, 1);
                        while(Wire.available() > 0){
                                key_ch = Wire.read();
                                if(key_ch != (char)0x00){
                                        if(playHandle){
                                                vTaskResume(playHandle);
                                        }
                                }
                        }
                        return key_ch;
                }

		static void mouse_read(lv_indev_t *indev, lv_indev_data_t *data){
                        static  int16_t last_x;
                        static int16_t last_y;
                        bool left_button_down = false;
                        const uint8_t dir_pins[5] = {
                                BOARD_TBOX_G02,
                                BOARD_TBOX_G01,
                                BOARD_TBOX_G04,
                                BOARD_TBOX_G03,
                                BOARD_BOOT_PIN
                        };
                        static bool last_dir[5];
                        uint8_t pos = 10;
                        for (int i = 0; i < 5; i++) {
                                bool dir = digitalRead(dir_pins[i]);
                                if (dir != last_dir[i]) {
                                        last_dir[i] = dir;
                                        switch (i) {
                                                case 0:
                                                        screenLockConditionBall = false;
                                                        if(last_x < (lv_disp_get_hor_res(NULL) - mousePointerPng.header.w)){
                                                                last_x += pos;
                                                        }
                                                        break;
                                                case 1:
                                                        screenLockConditionBall = false;
                                                        if(last_y > mousePointerPng.header.h){
                                                                last_y -= pos;
                                                        }
                                                        break;
                                                case 2:
                                                        screenLockConditionBall = false;
                                                        if(last_x > mousePointerPng.header.w){
                                                                last_x -= pos;
                                                        }
                                                        break;
                                                case 3:
                                                        screenLockConditionBall = false;
                                                        if(last_y < (lv_disp_get_ver_res(NULL) - mousePointerPng.header.h)){
                                                                last_y += pos;
                                                        }
                                                        break;
						case 4:
                                                        screenLockConditionBall = false;
                                                        left_button_down = true;
                                                        screenLockConditionBall  = true;
                                                        break;
                                                default:
                                                        break;
                                        }
                                }
                        }
                        // Serial.printf("indev:X:%04d  Y:%04d \n", last_x, last_y);
                        /*Store the collected data*/
                        data->point.x = last_x;
                        data->point.y = last_y;
                        data->state = left_button_down ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
                }
	public:	
		void pinInit(void){
			Serial.begin(115200);
			this->powerInit();
			this->spiInit();
			this->miscInit();
			this->wakupTouchInit();
		}
		
		void aceButtonInit(void){
			button.init();
			ButtonConfig *buttonConfig = button.getButtonConfig();
                        buttonConfig->setEventHandler(callback_acebutton);
                        buttonConfig->setFeature(ButtonConfig::kFeatureClick);
                        buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
		}

		void semaphoreInit(void){
			//Add mutex to allow multitasking access
                        xSemaphore = xSemaphoreCreateBinary();
                        assert(xSemaphore);
                        xSemaphoreGive( xSemaphore );
		}

		void lcdInit(void){
			tft.begin();
                        tft.setRotation( 1 );
                        tft.fillScreen(TFT_BLACK);
                        Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
		}

		void touchscreenInit(void){
			pinMode(BOARD_TOUCH_INT, INPUT); 
			delay(20); // Allow time for configuration change.
			this->scanForTouchDevices(&Wire);
			touch = new TouchLib(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, touchAddress);
			touch->init();
			Wire.beginTransmission(touchAddress);
			touchDected = Wire.endTransmission() == 0;
		}

		void lvglInit(void){
			lv_tick_set_cb(&callback_calcTick);
			lv_init();
			Serial.printf("Creating display...\n");
			display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
			if(display == NULL){
				throw CoreException("ESP32Initalizer::lvglInit() - Failed to create Display", 0x01);
			}
			lv_display_set_resolution(display, SCREEN_HOR, SCREEN_VIR);
			lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
			Serial.printf("Creating draw buffer...\n");
			draw_buf = (lv_color_t *)ps_malloc(LVGL_BUFFER_SIZE);
			if(draw_buf == NULL){
				throw CoreException("ESP32Initalizer::lvglInit() - Failed to allowcate draw buffer in PSRAM.", 0x02);
			}
			lv_display_set_buffers(display, draw_buf, NULL, sizeof(draw_buf)*LVGL_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_PARTIAL);
			lv_display_set_flush_cb(display, &disp_flush);

			Serial.printf("Creating touchpad input device.\n");
			indev_touchpad = lv_indev_create();
			if(indev_touchpad == NULL){
				throw CoreException("ESP32Initalizer::lvglInit() - Failed to create touchpad input device.", 0x03);
			}
			lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
			lv_indev_set_read_cb(indev_touchpad, &touchpad_read);

			Serial.printf("Creating keyboard input device.\n");
			indev_keypad = lv_indev_create();
			if(indev_keypad == NULL){
				throw CoreException("ESP32Initalizer::lvglInit() - Failed to create keypad input device.", 0x04);
			}
			lv_indev_set_type(indev_keypad, LV_INDEV_TYPE_KEYPAD);
			lv_indev_set_read_cb(indev_keypad, &keypad_read);

			Serial.printf("Creating mouse input device.\n");
			indev_mouse = lv_indev_create();
			lv_indev_set_type(indev_mouse, LV_INDEV_TYPE_POINTER);
			lv_indev_set_read_cb(indev_mouse, &mouse_read);
			cursorImg = lv_img_create(lv_scr_act());
			if(cursorImg == NULL){
				throw CoreException("ESP32Initalizer::lvglInit() - Failed to create mouse cursor image.", 0x05);
			}
			lv_img_set_src(cursorImg, &mousePointerPng);
			lv_indev_set_cursor(indev_mouse, cursorImg);
		}

		bool setupSD(){
                        digitalWrite(BOARD_SDCARD_CS, HIGH);
                        digitalWrite(RADIO_CS_PIN, HIGH);
                        digitalWrite(BOARD_TFT_CS, HIGH);

                        if(SD.begin(BOARD_SDCARD_CS, SPI, 800000U)){
                                uint8_t cardType = SD.cardType();
                                if(cardType == CARD_NONE){
                                        Serial.println("No SD_MMC card attached");
                                        return false;
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
                                        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
                                        uint32_t cardTotal = SD.totalBytes() / (1024 * 1024);
                                        uint32_t cardUsed = SD.usedBytes() / (1024 * 1024);
                                        Serial.printf("SD Card Size: %lu MB\n", cardSize);
                                        Serial.printf("Total space: %lu MB\n",  cardTotal);
                                        Serial.printf("Used space: %lu MB\n",   cardUsed);
                                        return true;
                                }
                        }
                        return false;
                }

		bool setupRadio(){
                        digitalWrite(BOARD_SDCARD_CS, HIGH);
                        digitalWrite(RADIO_CS_PIN, HIGH);
                        digitalWrite(BOARD_TFT_CS, HIGH);
                        SPI.end();
                        SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI); //SD

                        int state = radio.begin(RADIO_FREQ);
                        if(state == RADIOLIB_ERR_NONE){
                                Serial.println("Start Radio success!");
                        }else{
                                Serial.print("Start Radio failed,code:");
                                Serial.println(state);
                                return false;
                        }

                        hasRadio = true;

                        // set carrier frequency to 868.0 MHz
                        if(radio.setFrequency(RADIO_FREQ) == RADIOLIB_ERR_INVALID_FREQUENCY){
                                Serial.println(F("Selected frequency is invalid for this module!"));
                                return false;
                        }

                        // set bandwidth to 250 kHz
                        if(radio.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH){
                                Serial.println(F("Selected bandwidth is invalid for this module!"));
                                return false;
                        }

                        // set spreading factor to 10
                        if(radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR){
                                Serial.println(F("Selected spreading factor is invalid for this module!"));
                                return false;
                        }

                        // set coding rate to 6
                        if(radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE){
                                Serial.println(F("Selected coding rate is invalid for this module!"));
                                return false;
                        }

			// set LoRa sync word to 0xAB
                        if(radio.setSyncWord(0xAB) != RADIOLIB_ERR_NONE){
                                Serial.println(F("Unable to set sync word!"));
                                return false;
                        }

                        // set output power to 10 dBm (accepted range is -17 - 22 dBm)
                        if(radio.setOutputPower(17) == RADIOLIB_ERR_INVALID_OUTPUT_POWER){
                                Serial.println(F("Selected output power is invalid for this module!"));
                                return false;
                        }

                        // set over current protection limit to 140 mA (accepted range is 45 - 140 mA)
                        // NOTE: set value to 0 to disable overcurrent protection
                        if(radio.setCurrentLimit(140) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT){
                                Serial.println(F("Selected current limit is invalid for this module!"));
                                return false;
                        }

                        // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
                        if(radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH){
                                Serial.println(F("Selected preamble length is invalid for this module!"));
                                return false;
                        }

                        // disable CRC
                        if(radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION){
                                Serial.println(F("Selected CRC is invalid for this module!"));
                                return false;
                        }

			// set the function that will be called
                        // when new packet is received
                        //radio.setDio1Action(Setup_setTxFlag);
                        //radio.setPacketSentAction(Setup_setTxFlag);
                        radio.setPacketReceivedAction(Setup_setRxFlag);

                        int err = radio.startReceive();
                        if(state == RADIOLIB_ERR_NONE){
                                Serial.println(F("success!"));
                        }else{
                                Serial.print(F("failed, code "));
                                Serial.println(state);
                                while (true);
                        }
                        // Run listen mode by default
                        return true;
                }

}initer;
