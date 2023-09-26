lv_obj_t *Setup_usernameTxt;
lv_obj_t *Setup_passwordTxt;
int Setup_setupControl = 0;
int Sleep_interactionCtx = 0;
int Sleep_maxBrightness = 256;
int Sleep_brightness = 0;
float Sleep_timer = millis();
char compBuffer[__GLOBAL_BUFFER_SIZE] = {0};

#include "palCrypto.h"

static void Setup_setRxFlag(void){
  rxFlag = true;
}

static void Setup_setTxFlag(void){
  // we got a packet, set the flag
  txFlag = true;
  transmissionFlag = false;
}

class PalcomSetup{
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
      disp_drv.flush_cb = disp_flush;
      disp_drv.draw_buf = &draw_buf;
  #ifdef BOARD_HAS_PSRAM
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

      if (SD.begin(BOARD_SDCARD_CS, SPI, 800000U)) {
          uint8_t cardType = SD.cardType();
          if (cardType == CARD_NONE) {
              Serial.println("No SD_MMC card attached");
              return false;
          } else {
              Serial.print("SD_MMC Card Type: ");
              if (cardType == CARD_MMC) {
                  Serial.println("MMC");
              } else if (cardType == CARD_SD) {
                  Serial.println("SDSC");
              } else if (cardType == CARD_SDHC) {
                  Serial.println("SDHC");
              } else {
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
      for (addr = 1; addr < 127; addr++) {
          start = millis();
          w->beginTransmission(addr); delay(2);
          err = w->endTransmission();
          if (err == 0) {
              nDevices++;
              Serial.print("I2C device found at address 0x");
              if (addr < 16) {
                  Serial.print("0");
              }
              Serial.print(addr, HEX);
              Serial.println(" !");

              if (addr == GT911_SLAVE_ADDRESS2) {
                  touchAddress = GT911_SLAVE_ADDRESS2;
                  Serial.println("Find GT911 Drv Slave address: 0x14");
              } else if (addr == GT911_SLAVE_ADDRESS1) {
                  touchAddress = GT911_SLAVE_ADDRESS1;
                  Serial.println("Find GT911 Drv Slave address: 0x5D");
              }
          } else if (err == 4) {
              Serial.print("Unknow error at address 0x");
              if (addr < 16) {
                  Serial.print("0");
              }
              Serial.println(addr, HEX);
          }
      }
      if (nDevices == 0)
          Serial.println("No I2C devices found\n");
    }

    static uint32_t keypad_get_key(void){
        char key_ch = 0;
        Wire.requestFrom(0x55, 1);
        while (Wire.available() > 0) {
            key_ch = Wire.read();
            if (key_ch != (char)0x00) {
                if (playHandle) {
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
        if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
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
        if (!touch->read()) {
            return false;
        }
        TP_Point t = touch->getPoint(0);
        switch (rotation) {
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
        const uint8_t dir_pins[5] = {BOARD_TBOX_G02,
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
                    if (last_x < (lv_disp_get_hor_res(NULL) - mouse_cursor_icon.header.w)) {
                        last_x += pos;
                    }
                    break;
                case 1:
                    if (last_y > mouse_cursor_icon.header.h) {
                        last_y -= pos;
                    }
                    break;
                case 2:
                    if (last_x > mouse_cursor_icon.header.w) {
                        last_x -= pos;
                    }
                    break;
                case 3:
                    if (last_y < (lv_disp_get_ver_res(NULL) - mouse_cursor_icon.header.h)) {
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
      if (state == RADIOLIB_ERR_NONE) {
          Serial.println("Start Radio success!");
      } else {
          Serial.print("Start Radio failed,code:");
          Serial.println(state);
          return false;
      }

      hasRadio = true;

      // set carrier frequency to 868.0 MHz
      if (radio.setFrequency(RADIO_FREQ) == RADIOLIB_ERR_INVALID_FREQUENCY) {
          Serial.println(F("Selected frequency is invalid for this module!"));
          return false;
      }

      // set bandwidth to 250 kHz
      if (radio.setBandwidth(250.0) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
          Serial.println(F("Selected bandwidth is invalid for this module!"));
          return false;
      }

      // set spreading factor to 10
      if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
          Serial.println(F("Selected spreading factor is invalid for this module!"));
          return false;
      }

      // set coding rate to 6
      if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
          Serial.println(F("Selected coding rate is invalid for this module!"));
          return false;
      }

      // set LoRa sync word to 0xAB
      if (radio.setSyncWord(0xAB) != RADIOLIB_ERR_NONE) {
          Serial.println(F("Unable to set sync word!"));
          return false;
      }

      // set output power to 10 dBm (accepted range is -17 - 22 dBm)
      if (radio.setOutputPower(17) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
          Serial.println(F("Selected output power is invalid for this module!"));
          return false;
      }

      // set over current protection limit to 140 mA (accepted range is 45 - 140 mA)
      // NOTE: set value to 0 to disable overcurrent protection
      if (radio.setCurrentLimit(140) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
          Serial.println(F("Selected current limit is invalid for this module!"));
          return false;
      }

      // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
      if (radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
          Serial.println(F("Selected preamble length is invalid for this module!"));
          return false;
      }

      // disable CRC
      if (radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
          Serial.println(F("Selected CRC is invalid for this module!"));
          return false;
      }

      // set the function that will be called
      // when new packet is received
      //radio.setDio1Action(Setup_setTxFlag);
      //radio.setPacketSentAction(Setup_setTxFlag);
      radio.setPacketReceivedAction(Setup_setRxFlag);

      int err = radio.startReceive();
      if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
      } else {
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
        if (act_key != 0) {
            data->state = LV_INDEV_STATE_PR;
            Sleep_interactionCtx = 0;
            Sleep_timer = millis();
            //Serial.printf("Key pressed : 0x%x\n", act_key);
            last_key = act_key;
        } else {
            data->state = LV_INDEV_STATE_REL;
        }
        data->key = last_key;
    }
    static void Setup_handleAceEvent(AceButton * /* button */, uint8_t eventType, uint8_t /* buttonState */){
      switch (eventType) {
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
        string username = lv_textarea_get_text(Setup_usernameTxt);
        string password = lv_textarea_get_text(Setup_passwordTxt);
        
        if(username == "" || password == ""){
          Setup_setupControl = 2;
          return;
        }
        char pass_hash[33];
        getSha256Hash((char*)password.c_str(), password.length(), pass_hash);
        for(int i=0; i<100000; i++)
          fileData[i] = 0;
        int iter = 0;
        for(int i=0;i<username.length(); i++){
          fileData[iter] = username[i];
          iter++;
        }
        fileData[iter] = '\r';
        iter++;
        fileData[iter] = '\n';
        iter++;
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
    
    lv_obj_t *setup_cont = NULL;

  public:
    
    bool systemSetup(lv_obj_t *parent){ 
      if (SD.exists(F("/login.hash"))) {
        return true;
      }
      setup_cont = lv_obj_create(parent);
      lv_obj_set_size(setup_cont, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
      lv_obj_set_scroll_dir(setup_cont, LV_DIR_VER);
      // Section title
      lv_obj_t *label;
      label = lv_label_create(setup_cont);
      lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
      lv_obj_set_width(label, 320);
      if(Setup_setupControl == 0)
        lv_label_set_text(label, "Please setup your account!");
      else
        lv_label_set_text(label, "Please setup your account! || user and password required");
      lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);

      // Username section
      label = lv_label_create(setup_cont);
      lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
      lv_obj_set_width(label, 320);
      lv_label_set_text(label, "Username: ");
      lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 35);
    
      static lv_style_t ta_bg_style;
      lv_style_init(&ta_bg_style);
      lv_style_set_text_color(&ta_bg_style, lv_color_white());
      lv_style_set_bg_opa(&ta_bg_style, LV_OPA_100);

      Setup_usernameTxt = lv_textarea_create(setup_cont);
      lv_textarea_set_cursor_click_pos(Setup_usernameTxt, false);
      lv_textarea_set_text_selection(Setup_usernameTxt, false);
      lv_obj_set_size(Setup_usernameTxt, 175, 23);
      lv_textarea_set_text(Setup_usernameTxt, "");
      lv_textarea_set_max_length(Setup_usernameTxt, 18);
      lv_textarea_set_one_line(Setup_usernameTxt, true);
      lv_obj_align(Setup_usernameTxt, LV_ALIGN_TOP_MID, 20, 30);

      lv_obj_add_style(Setup_usernameTxt, &ta_bg_style, LV_PART_ANY);

      // Password Section
      label = lv_label_create(setup_cont);
      lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
      lv_obj_set_width(label, 320);
      lv_label_set_text(label, "Password: ");
      lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 70+10);

      Setup_passwordTxt = lv_textarea_create(setup_cont);
      lv_textarea_set_cursor_click_pos(Setup_passwordTxt, false);
      lv_textarea_set_text_selection(Setup_passwordTxt, false);
      lv_obj_set_size(Setup_passwordTxt, 175, 23);
      lv_textarea_set_text(Setup_passwordTxt, "");
      lv_textarea_set_max_length(Setup_passwordTxt, 18);
      lv_textarea_set_one_line(Setup_passwordTxt, true);
      lv_textarea_set_password_mode(Setup_passwordTxt, true);
      lv_obj_align(Setup_passwordTxt, LV_ALIGN_TOP_MID, 20, 60+10);
      lv_obj_add_style(Setup_passwordTxt, &ta_bg_style, LV_PART_ANY);

      // submit button
      lv_obj_t *submitButton = lv_btn_create(setup_cont);
      lv_obj_set_size(submitButton, LV_PCT(100), LV_PCT(40));//LV_PCT(LV_HOR_RES), LV_PCT(40));
      lv_obj_t *submitButton_label = lv_label_create(submitButton);
      lv_label_set_text(submitButton_label, "Setup");
      lv_obj_center(submitButton_label);
      lv_obj_add_event_cb(submitButton, Setup_handleSubmit, LV_EVENT_ALL, 0);
      lv_obj_align_to(submitButton, submitButton_label, LV_ALIGN_OUT_BOTTOM_MID, -5,  70);

      Setup_setupControl = 0;
      while(Setup_setupControl != 1){
        lv_task_handler(); delay(5);
        if(Setup_setupControl == 2){
          return false;
        }
      }
    
      setup_cont = lv_obj_create(parent);
      lv_obj_set_size(setup_cont, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
      lv_obj_set_scroll_dir(setup_cont, LV_DIR_VER);
      // Section title
      label = lv_label_create(setup_cont);
      lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
      lv_obj_set_width(label, 320);
      lv_label_set_text(label, "Generating Encryption Keys.");
      lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);
      for(int i=0; i<100; i++){
         lv_task_handler(); delay(5);
      }
      
      generateKeyPair(true);
      
      setup_cont = lv_obj_create(parent);
      lv_obj_set_size(setup_cont, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
      lv_obj_set_scroll_dir(setup_cont, LV_DIR_VER);
      // Section title
      label = lv_label_create(setup_cont);
      lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
      lv_obj_set_width(label, 320);
      lv_label_set_text(label, "Generation successful! Tap the screen.");
      lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 10);
      for(int i=0; i<100; i++){
         lv_task_handler(); delay(5);
      }
      return true;
    }

  void resetLvgl(){
    setupLvgl();
  }
  void systemInit(){
     bool ret = 0;

    Serial.begin(115200);
    Serial.println("Palcom LoRa Phone");

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

    button.init();
    ButtonConfig *buttonConfig = button.getButtonConfig();
    buttonConfig->setEventHandler(Setup_handleAceEvent);
    buttonConfig->setFeature(ButtonConfig::kFeatureClick);
    buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);

    //Add mutex to allow multitasking access
    xSemaphore = xSemaphoreCreateBinary();
    assert(xSemaphore);
    xSemaphoreGive( xSemaphore );

    tft.begin();
    tft.setRotation( 1 );
    tft.fillScreen(TFT_BLACK);
    Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);

    // Set touch int input
    pinMode(BOARD_TOUCH_INT, INPUT); delay(20);

    // Two touch screens, the difference between them is the device address,
    // use ScanDevices to get the existing I2C address
    scanDevices(&Wire);

    touch = new TouchLib(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL, touchAddress);
    touch->init();

    Wire.beginTransmission(touchAddress);
    ret = Wire.endTransmission() == 0;
    touchDected = ret;

    kbDected = checkKb();

    setupLvgl();

    ret = setupSD();
    Serial.print("SDCard:"); Serial.println(ret);
    ret = setupRadio();
    Serial.print("Radio:"); Serial.println(ret);

  }
}palcomSetup;