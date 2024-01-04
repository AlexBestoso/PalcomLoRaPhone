lv_obj_t *Setup_usernameTxt;
lv_obj_t *Setup_passwordTxt;

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

int systemSetup_contextControl = 0;

class PalcomSetup : public PalcomScreen{
  	private:
    		/*
     		* Standard class functions.
     		*/
    		void setupLvgl(){
      			static lv_disp_draw_buf_t draw_buf;

      			#ifndef BOARD_HAS_PSRAM
      				#define LVGL_BUFFER_SIZE    ( TFT_HEIGHT * 100 )
          			static lv_color_t buf[ LVGL_BUFFER_SIZE ];
      			#else
      				#define LVGL_BUFFER_SIZE    (TFT_WIDTH * TFT_HEIGHT * sizeof(lv_color_t))
          			static lv_color_t *buf = (lv_color_t *)ps_malloc(LVGL_BUFFER_SIZE);
          			if (!buf) {
              				Serial.println("menory alloc failed!");
              				delay(5000);
              				assert(buf);
          			}
      			#endif

      			lv_init();
      			lv_group_set_default(lv_group_create());
	      		lv_disp_draw_buf_init( &draw_buf, buf, NULL, LVGL_BUFFER_SIZE );

	      		/*Initialize the display*/
	      		static lv_disp_drv_t disp_drv;
	      		lv_disp_drv_init( &disp_drv );

	      		/*Change the following line to your display resolution*/
	      		disp_drv.hor_res = TFT_HEIGHT;
	      		disp_drv.ver_res = TFT_WIDTH;
			//
	      		disp_drv.flush_cb = disp_flush;
	      		disp_drv.draw_buf = &draw_buf;
	  		#ifdef BOARD_HAS_PSRAM
				Serial.printf("PSRAM Full Refresh Enabled\m");
	      			disp_drv.full_refresh = 1;
	  		#endif
	      		lv_disp_drv_register( &disp_drv );
	
	      		/*Initialize the  input device driver*/
	
	      		/*Register a touchscreen input device*/
	      		if (touchDected) {
          			static lv_indev_drv_t indev_touchpad;
          			lv_indev_drv_init( &indev_touchpad );
          			indev_touchpad.type = LV_INDEV_TYPE_POINTER;
          			indev_touchpad.read_cb = touchpad_read;
          			touch_indev = lv_indev_drv_register( &indev_touchpad );
      			}

      			/*Register a mouse input device*/
      			static lv_indev_drv_t indev_mouse;
      			lv_indev_drv_init( &indev_mouse );
      			indev_mouse.type = LV_INDEV_TYPE_POINTER;
      			indev_mouse.read_cb = mouse_read;
      			mouse_indev = lv_indev_drv_register( &indev_mouse );
      			lv_indev_set_group(mouse_indev, lv_group_get_default());

      			lv_obj_t *cursor_obj;
      			cursor_obj = lv_img_create(lv_scr_act());         /*Create an  object for the cursor */
      			lv_img_set_src(cursor_obj, &mouse_cursor_icon);   /*Set the image source*/
      			lv_indev_set_cursor(mouse_indev, cursor_obj);           /*Connect the image  object to the driver*/

      			if (kbDected) {
      	    			Serial.println("Keyboard registered!!");
          			/*Register a keypad input device*/
          			static lv_indev_drv_t indev_keypad;
          			lv_indev_drv_init(&indev_keypad);
          			indev_keypad.type = LV_INDEV_TYPE_KEYPAD;
          			indev_keypad.read_cb = keypad_read;
          			kb_indev = lv_indev_drv_register(&indev_keypad);
          			lv_indev_set_group(kb_indev, lv_group_get_default());
      			}	
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
    	
		bool checkKb(){
      			Wire.requestFrom(0x55, 1);
      			return Wire.read() != -1;
    		}
    
		void scanDevices(TwoWire *w){
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
        		//Sleep_interactionCtx = 0;
        		//Sleep_timer = millis();
        		return key_ch;
    		}

    		static void disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p ){
        		uint32_t w = ( area->x2 - area->x1 + 1 );
        		uint32_t h = ( area->y2 - area->y1 + 1 );
			if(xSemaphore == NULL){
				Serial.printf("Semaphore is null\n");
				return;
			}
        		if( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ){
            			tft.startWrite();
            			tft.setAddrWindow( area->x1, area->y1, w, h );
            			tft.pushColors( ( uint16_t * )&color_p->full, w * h, false );
            			tft.endWrite();
            			lv_disp_flush_ready( disp );
            			xSemaphoreGive( xSemaphore );
        		}
    		}
    
		static bool getTouch(int16_t &x, int16_t &y){
        		uint8_t rotation = tft.getRotation();
        		if(!touch->read()){
            			return false;
        		}
        		TP_Point t = touch->getPoint(0);
        		switch(rotation){
        			case 1:
            				x = t.y;
            				y = tft.height() - t.x;
            				break;
        			case 2:
            				x = tft.width() - t.x;
            				y = tft.height() - t.y;
            				break;
        			case 3:
            				x = tft.width() - t.y;
            				y = t.x;
            				break;
        			case 0:
        			default:
            				x = t.x;
            				y = t.y;
        		}
        		Sleep_interactionCtx = 0;
        		Sleep_timer = millis();
        		//Serial.printf("R:%d X:%d Y:%d\n", rotation, x, y);
        		return true;
    		}
    
		/*
    	 	 * Static Setup Functions
   	 	 */
    		static void mouse_read(lv_indev_drv_t *indev, lv_indev_data_t *data){
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
                    					if(last_x < (lv_disp_get_hor_res(NULL) - mouse_cursor_icon.header.w)){
                        					last_x += pos;
                    					}
                    					break;
                				case 1:
                    					if(last_y > mouse_cursor_icon.header.h){
                        					last_y -= pos;
                    					}
                    					break;
                				case 2:
                    					if(last_x > mouse_cursor_icon.header.w){
                        					last_x -= pos;
                    					}
                    					break;
                				case 3:
                    					if(last_y < (lv_disp_get_ver_res(NULL) - mouse_cursor_icon.header.h)){
                        					last_y += pos;
                    					}
                    					break;
                				case 4:
                    					left_button_down = true;
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
	    
    		/*Will be called by the library to read the mouse*/
    		static void keypad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data){
        		static uint32_t last_key = 0;
       			uint32_t act_key ;
        		act_key = keypad_get_key();
        		if(act_key != 0){
            			data->state = LV_INDEV_STATE_PR;
            			Sleep_interactionCtx = 0;
            			Sleep_timer = millis();
            			//Serial.printf("Key pressed : 0x%x\n", act_key);
            			last_key = act_key;
        		}else{
            			data->state = LV_INDEV_STATE_REL;
        		}
        		data->key = last_key;
    		}
    
		static void Setup_handleAceEvent(AceButton * /* button */, uint8_t eventType, uint8_t /* buttonState */){
     			switch(eventType){
        			case AceButton::kEventClicked:
            				clicked = true;
            				//Serial.println("Clicked!");
            				break;
        			case AceButton::kEventLongPressed:
          				//Serial.println("ClickkEventLongPresseded!"); delay(2000);
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

    		static void touchpad_read( lv_indev_drv_t *indev_driver, lv_indev_data_t *data ){
      			data->state = getTouch(data->point.x, data->point.y) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
    		}

    		static void Setup_handleSubmit(lv_event_t *e){
      			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
        			PalcomTextarea login_user;
        			PalcomTextarea login_pass;
				PalcomTextarea login_passConfirm;
        			login_user.loadGlobal(1);
        			login_pass.loadGlobal(2);
				login_passConfirm.loadGlobal(3);
        			string username = login_user.getText();
        			string password = login_pass.getText();
				string confirmPassword = login_passConfirm.getText();

        			if(username == "" || password == ""){
          				Setup_setupControl = 0;
					PalcomScreenError = 2;
          				Serial.printf("No user or password.\n");
          				return;
        			}

				if(password != confirmPassword){
					Setup_setupControl = 0;
					Serial.printf("Passwords don't match.\n");
					PalcomScreenError = 1;
					return;
				}

				PalcomFS pfs;
	                        pfs.setCallsign(login_user.getText());

				Serial.printf("Creating login file.\n");
				pfs.clearAllBuffers();
        			char pass_hash[33];
        			getSha256Hash((char*)password.c_str(), password.length(), pass_hash);
        			for(int i=0;i<username.length(); i++){
          				fileData[i] = username[i];
        			}
        			size_t iter = username.length();
        			fileData[iter] = '\r';
        			fileData[iter+1] = '\n';
        			iter+=2;

        			for(int i=0; i<33; i++){
          				fileData[iter] = pass_hash[i];
					iter++;
        			}

        			File hashFile = SD.open("/login.hash", FILE_WRITE);
        			if(!hashFile){
          				Serial.println("Failed to open file.");
          				while(1){delay(5);}
        			}
      
        			hashFile.printf("%s", (const char *)fileData);
        			hashFile.close();

        			Setup_setupControl = 1;
      			}
    		}
    
    		void initPins(){
     			Serial.begin(115200);

      			//! The board peripheral power control pin needs to be set to HIGH when using the peripheral
      			pinMode(BOARD_POWERON, OUTPUT);
      			digitalWrite(BOARD_POWERON, HIGH);

      			//! Set CS on all SPI buses to high level during initialization
      			pinMode(BOARD_SDCARD_CS, OUTPUT);
      			pinMode(RADIO_CS_PIN, OUTPUT);
      			pinMode(BOARD_TFT_CS, OUTPUT);

      			digitalWrite(BOARD_SDCARD_CS, HIGH);
      			digitalWrite(RADIO_CS_PIN, HIGH);
      			digitalWrite(BOARD_TFT_CS, HIGH);

      			pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
      			SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI); //SD

      			pinMode(BOARD_BOOT_PIN, INPUT_PULLUP);
      			pinMode(BOARD_TBOX_G02, INPUT_PULLUP);
      			pinMode(BOARD_TBOX_G01, INPUT_PULLUP);
      			pinMode(BOARD_TBOX_G04, INPUT_PULLUP);
      			pinMode(BOARD_TBOX_G03, INPUT_PULLUP);

      			//Wakeup touch chip
      			pinMode(BOARD_TOUCH_INT, OUTPUT);
      			digitalWrite(BOARD_TOUCH_INT, HIGH);
    		}

    		void initButton(){
      			button.init();
      			ButtonConfig *buttonConfig = button.getButtonConfig();
      			buttonConfig->setEventHandler(Setup_handleAceEvent);
      			buttonConfig->setFeature(ButtonConfig::kFeatureClick);
      			buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
    		}

    		void initSemaphore(){
      			//Add mutex to allow multitasking access
      			xSemaphore = xSemaphoreCreateBinary();
      			assert(xSemaphore);
      			xSemaphoreGive( xSemaphore );
    		}

    		void initScreen(){
      			tft.begin();
      			tft.setRotation( 1 );
      			tft.fillScreen(TFT_BLACK);
      			Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
      			setupLvgl();
    		}

		const lv_img_dsc_t *img_src[1] = {&palcomLogo};
    		void displaySplash(){
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);
      	
      			this->setBgImage(img_src);
			this->setBgX(20);
      			lv_task_handler();
    		}

    		void finalInit(){
      			// Set touch int input
      			pinMode(BOARD_TOUCH_INT, INPUT); delay(20);

      			// Two touch screens, the difference between them is the device address,
      			// use ScanDevices to get the existing I2C address
      			scanDevices(&Wire);

	      		touch = new TouchLib(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, touchAddress);
	      		touch->init();

	      		Wire.beginTransmission(touchAddress);
	      		bool ret = Wire.endTransmission() == 0;
	      		touchDected = ret;

	      		kbDected = checkKb();

	      		setupLvgl();

	     		ret = setupSD();
	     		ret = setupRadio();

	      		generatePublicHash(true);
	    	}

	    	void keygenView(){
			//displaySplash();
	        	lv_obj_t *screen = this->getScreen();
	        	if(screen == NULL){
		          	this->globalDestroy();
		          	this->create();
		          	screen = this->getScreen();
	        	}
	        	this->setFullScreen();
	        	this->setScreenScrollDirection(LV_DIR_VER);
			string msg = "Generating Key Pair\n";

			int y = 8;
			int x = 10;
	        	PalcomLabel pLabel;
	        	pLabel.create(screen);
	        	pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
	        	pLabel.setWidth(320);
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
	        	pLabel.setText(msg.c_str());
			lv_task_handler();

			PalcomCrypto pcry;
	        	//generateKeyPair(true);
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Initalizing RSA Variables...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
			pcry.rsaKeyGen.initalizeVars();
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating RNG Seed...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
                        pcry.rsaKeyGen.generateRngSeed();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating Key Pair...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
                        pcry.rsaKeyGen.generateKeyPair();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Exporting Key Pair...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
                        pcry.rsaKeyGen.exportKeys();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Writing Public Key To SD Card...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
                        pcry.rsaKeyGen.storeExportedPublicKey();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Writing Private Key To SD Card...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
                        pcry.rsaKeyGen.storeExportedPrivateKey();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();

			
			/*if(!pcry.rsaKeyGen.genKeys())
				Serial.printf("Key Generation Failed.\n");
			*/
			pcry.rsaKeyGen.freeVars();

			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating public key hash file...";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
			generatePublicHash(true);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        lv_task_handler();
       	 		Setup_setupControl = 1;
    		}

    		lv_obj_t *setup_cont = NULL;
    		bool initalized = false;
		bool loginFileExists = false;
		string usrname = "";

	public:
	    	bool buildRequired = true;

	    	void generateObjects(void){
			/*
			 * Initalize all the stuff required for the system to work.
			 * */
	      		if(!initalized){
	        		initPins();
	        		initButton();
       	 			initSemaphore();
        			initScreen();
        			displaySplash();
        			finalInit();
        			initalized = true;
				this->setBuildRequired(true);
				return;
      			}

			displaySplash();

			/*
			 * Check if we need to setup the system
			 * .*/
      			if(SD.exists(F("/login.hash"))){
				loginFileExists = true;
        			return;
			}

			/*
			 * Configure the screen for this context.
			 * */
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScrollMode(LV_SCROLLBAR_MODE_OFF);

			/*
			 * Setup the context title
			 * */
      			PalcomLabel pLabel;
      			pLabel.create(screen);
			lv_task_handler();
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
			int err = getScreenError();
			if(err == 1){
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 70, 8);
	        		pLabel.setText("Passwords don't match.");
			}else if(err == 0/*Setup_setupControl == 2*/ ){
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 100, 8);
	        		pLabel.setText("Create New Account");
      			}else{
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 70, 8);
      				pLabel.setText("User And Password Required.");
      			}
			lv_task_handler();

			/*
			 * Create username input
			 * */
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
      			pLabel.setAlignment(LV_ALIGN_TOP_MID, 5, 35);
      			pLabel.setText("Username:");
			lv_task_handler();

			string username_str = "";
      			PalcomTextarea username;
      			username.createGlobal(screen, 1);
			defaultTextareaStyle.initStyle();
			username.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
      			username.setCursorClickPos(false);
      			username.setTextSelection(false);
      			username.setSize(175, 23);
      			username.setText("");
      			username.setMaxLength(18);
      			username.setOneLine(true);
      			username.setAlignment(LV_ALIGN_TOP_MID, 20, 30);
			if(err == 1){
				username.setText(usrname.c_str());
			}
			lv_task_handler();

			/*
			 * Create password input
			 * */
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
      			pLabel.setAlignment(LV_ALIGN_TOP_MID, 5, 85);
      			pLabel.setText("Password: ");
			lv_task_handler();

      			PalcomTextarea password;
      			password.createGlobal(screen, 2);
			password.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
      			password.setCursorClickPos(false);
      			password.setTextSelection(false);
      			password.setSize(175, 23);
      			password.setText("");
      			password.setMaxLength(18);
      			password.setOneLine(true);
      			password.setPasswordMode(true);
      			password.setAlignment(LV_ALIGN_TOP_MID, 20, 75);
			lv_task_handler();

			/*
			 * Create confirm password input
			 * */
			pLabel.create(screen);
                        pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
                        pLabel.setWidth(320);
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, 5, 130);
                        pLabel.setText("Confirm: ");
			lv_task_handler();

                        PalcomTextarea confirmPassword;
                        confirmPassword.createGlobal(screen, 3);
			confirmPassword.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
                        confirmPassword.setCursorClickPos(false);
                        confirmPassword.setTextSelection(false);
                        confirmPassword.setSize(175, 23);
                        confirmPassword.setText("");
                        confirmPassword.setMaxLength(18);
                        confirmPassword.setOneLine(true);
                        confirmPassword.setPasswordMode(true);
                        confirmPassword.setAlignment(LV_ALIGN_TOP_MID, 20, 120);
			this->execute();


			/*
			 * Create Submit Button
			 * */

      			PalcomButton submit;
      			submit.create(screen);
			defaultButtonStyle.initStyle();
			submit.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
      			submit.setSize(105, 30);

      			pLabel.create(submit.getObj());
      			pLabel.setText("Setup");
      			pLabel.center();
      			
			submit.setLabel(pLabel);
      			submit.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, -5, 128);
      			submit.setSimpleCallback(Setup_handleSubmit);
			this->execute();
    		}

    		void resetPage(){
			this->setBuildRequired(true);
        		systemSetup_contextControl = 0;
        		this->globalDestroy();
        		this->destroy();
			Setup_setupControl = 0;
			loginFileExists = false;
    		}

    		int run(){
			/*
			 * Ensure page is setup
			 * */
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			systemSetup_contextControl = 0;
        			this->load();
      			}
      			lv_task_handler();

			/*
			 * Handle the setup acount context.
			 * This displays the key-gen info page.
			 * */
      			if(Setup_setupControl == 1){
				this->globalDestroy();
                        	this->destroy();
                        	lv_task_handler();
                        	keygenView();
                        	generatePublicHash(true);
        			resetPage();
        			return 0; // change screen to login page.
      			}

			/*
			 * If login file exists, exit setup page.
			 * */
			lv_task_handler();
      			if(loginFileExists) {
        			resetPage();
        			return 0; // change screen to login page.
      			}
      			lv_task_handler();

			/*
			 * Handle form validation errors.
			 * */
			if(isScreenError()){
				this->setBuildRequired(true);
				PalcomTextarea tmp;
				tmp.loadGlobal(1);
				usrname = tmp.getText();
				this->globalDestroy();
                        	this->destroy();
			}

      			return -1; // loop the setup page.
    		}

  		/*void resetLvgl(){
    			setupLvgl();
  		} */
}palcomSetup;
