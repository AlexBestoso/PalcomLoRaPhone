static uint32_t keypad_get_key(void){
	char key_ch = 0;
	Wire.requestFrom(0x55, 1);
	while(Wire.available() > 0){
		key_ch = Wire.read();
	//if(key_ch != (char)0x00){
	//	if(playHandle){
	//		vTaskResume(playHandle);
	//	}
	//}
	}
	return key_ch;
}

static uint32_t callback_calcTick(void){
	return millis();
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
		//tft.begin();
		//lv_draw_sw_rgb565_swap((void *)color_p, s);
		tft.startWrite();
		tft.setAddrWindow( area->x1, area->y1, w, h );
		tft.pushPixelsDMA((uint16_t *)color_p, w*h);
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
	
	data->state = LV_INDEV_STATE_PR;
	return;
}

static void keypad_read(lv_indev_t *indev_drv, lv_indev_data_t *data){
	static uint32_t last_key = 0;
        uint32_t act_key ;
        act_key = keypad_get_key();
        if(act_key != 0){
                data->state = LV_INDEV_STATE_PRESSED;

                last_key = act_key;
                if(keyboardFocusedObj != NULL){
                        PalcomTextarea kfo;
                        kfo.setObject(keyboardFocusedObj);
                        if(kfo.stateInUse(LV_STATE_FOCUSED)){
                                if(last_key == LV_KEY_BACKSPACE){
                                        kfo.popCharLeft();
                                }else{
                                        kfo.pushChar(last_key);
                                }
                        }
                }
        }else{
                data->state = LV_INDEV_STATE_RELEASED;
        }
        data->key = last_key;
}

static void scanForTouchDevices(TwoWire *w){
	if(w == NULL){
		Serial.printf("two wire w is null\n");
	}
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
