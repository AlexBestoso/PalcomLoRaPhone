enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;


bool settingsMenu_exit = false;
bool settingsMenu_newPin = false;
bool settingsMenu_updateTimer = false;
bool settingsMenu_updateBrightness = false;
class SettingsMenu : public PalcomMenu{
	private:
		PalcomPage generalPage;
		PalcomPage securityPage;
		PalcomPage relayPage;
		PalcomPage resetPage;
		PalcomPage rootPage;

		PalcomButton backBtn;
		PalcomObject rootPg;
		

		static void callback_exitSettingsMenu(lv_event_t * e){
			PalcomEvent event(e);
    			lv_obj_t *obj = (lv_obj_t*)event.getTarget();
    			lv_obj_t *menu = (lv_obj_t*)event.getUserData();

    			if(lv_menu_back_button_is_root(menu, obj) && event.getCode() == LV_EVENT_RELEASED) {
				settingsMenu_exit = true;
    			}
		}

		static void callback_wipeDevice(lv_event_t *e){
			PalcomEvent event(e);
			if(event.getCode() == LV_EVENT_RELEASED){
				PalcomFS pfs;
				pfs.rm("/");
				PalcomPartition pp;
				pp.fetchPartitionByName("app1");
				pp.eraseRange((const esp_partition_t *)pp.partition, 0, pp.partition->size);
				pp.freePartitions();
	
				delay(500);
				esp_restart();
			}
		}
		static void callback_factoryReset(lv_event_t *e){
			PalcomEvent event(e);
			if(event.getCode() == LV_EVENT_RELEASED){
				PalcomMessageBox mb;
				mb.create(lv_screen_active(), "WARNING", "Are you Sure that you want to continue? This will erase everything on the device.", true);
				mb.setFooterButton("Wipe", &callback_wipeDevice, 0);
				
			}
		}

		static void callback_newPasscode(lv_event_t *e){
			PalcomEvent event(e);
			if(event.getCode() == LV_EVENT_RELEASED){
				settingsMenu_newPin = true;
			}
		}

		static void callback_processUsername(lv_event_t *e){
			PalcomEvent event(e);

			if(event.getCode() == LV_EVENT_DEFOCUSED){
				PalcomTextarea textarea;
				textarea.setObject((lv_obj_t *)event.getTarget());
				String originalText = (const char *)event.getUserData();
				String newText = (const char *)textarea.getText();
				if(newText == "" || originalText == newText){
					return;
				}else if(newText != originalText){
					PalcomFS pfs;
					palcom_config_t *config = pfs.getConfigData();
					for(int i=0; i<20; i++){
						if(i<newText.length()){
							config->user_name[i] = newText[i];
						}else{
							config->user_name[i] = 0x00;
						}
					}
					pfs.storeConfigData(config[0]);
				}
			}
		}
		static void callback_processLockTimer(lv_event_t *e){
                        PalcomEvent event(e);

                        if(event.getCode() == LV_EVENT_DEFOCUSED){
                                PalcomTextarea textarea;
                                textarea.setObject((lv_obj_t *)event.getTarget());
                                uint8_t *originalValue = (uint8_t *)event.getUserData();
                                String newText = (const char *)textarea.getText();
				uint8_t newValue = newText.toInt();

                                if(newText == "" || originalValue[0] == newValue){
                                        return;
                                }else if(newValue != originalValue[0]){
                                        PalcomFS pfs;
                                        palcom_config_t *config = pfs.getConfigData();
                                        config->lock_timer = newValue;
                                        pfs.storeConfigData(config[0]);
					settingsMenu_updateTimer = true;
                                }
                        }
                }

		static void callback_processBrightness(lv_event_t *e){
			PalcomEvent event(e);

			if(event.getCode() == LV_EVENT_RELEASED){
				PalcomSlider slider;
				slider.setObject((lv_obj_t *)event.getTarget());
				PalcomFS pfs;
				palcom_config_t *config = pfs.getConfigData();
				config->screen_brightness = slider.getValue();
				pfs.storeConfigData(config[0]);
				analogWrite(BOARD_TFT_BACKLIGHT, config->screen_brightness);

				settingsMenu_updateBrightness = true;
			}
		}

		static void callback_togglePmode(lv_event_t *e){
			PalcomEvent event(e);
			if(event.getCode() == LV_EVENT_VALUE_CHANGED){
				PalcomSwitch sw;
				sw.setObject((lv_obj_t*)event.getTarget());	
				int newVal = sw.stateInUse(LV_STATE_CHECKED) ? 1 : 0;
				PalcomPartition pp;
				palcom_auth_t data;
				pp.fetchPartitionByName("app1");
				pp.readAuthData((const esp_partition_t *)pp.partition, &data);
				data.paranoia_mode = newVal;
				pp.writeAuthData((const esp_partition_t *)pp.partition, data);
				pp.freePartitions();
			}
		}

		lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt, lv_menu_builder_variant_t builder_variant){
    			lv_obj_t * obj = this->createContainer(parent);
		
    			lv_obj_t * img = NULL;
    			PalcomLabel label;

    			if(icon) {
        			img = lv_img_create(obj);
        			lv_img_set_src(img, icon);
    			}

    			if(txt) {
        			label.create(obj);
        			label.setText(txt);
        			label.setLongMode(LV_LABEL_LONG_SCROLL_CIRCULAR);
        			label.setFlexGrow(1);
    			}

    			if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        			lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        			lv_obj_swap(img, label.getObject());
    			}

    			return obj;
		}

		lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max, int32_t val){
    			lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

			PalcomSlider slider;
			slider.create(obj);
    			slider.setFlexGrow(1);
    			slider.setRange(min, max);
    			slider.setValue(val);
			slider.setParamCallback(&callback_processBrightness, slider.getObject());

    			if(icon == NULL) {
				slider.addFlag(LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    			}

    			return obj;
		}

		lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk){
    			lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

			PalcomSwitch sw;
    			sw.create(obj);
			if(chk){
				sw.addState(LV_STATE_CHECKED);
			}else if(sw.stateInUse(LV_STATE_CHECKED)){
				sw.removeState(LV_STATE_CHECKED);
			}

			sw.setSimpleCallback(&callback_togglePmode);

    			return obj;
		}

		lv_obj_t *create_button(lv_obj_t * parent, const char * icon, const char * txt, void(*func)(lv_event_t *e)){
    			lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);
			PalcomButton button;
			button.create(obj);
			button.setSize(90, 40);
			button.setSimpleCallback(func);
			PalcomLabel label;
			label.create(button.getObject());
			label.setText("");
			label.center();
			button.setLabel(label);

			return obj;
		}

		lv_obj_t *create_textarea(lv_obj_t *parent, const char *icon, const char *text, char *value=NULL){
			lv_obj_t *obj = this->create_text(parent, icon, text, LV_MENU_ITEM_BUILDER_VARIANT_2);
			PalcomTextarea textarea;
			textarea.create(obj);
			textarea.setSize(120, 40);
			if(value != NULL){
				textarea.setText((const char *)value);
			}
			

			return obj;

		}

		lv_obj_t *create_textarea2(lv_obj_t *parent, const char *icon, const char *text, char *value=NULL){
                        lv_obj_t *obj = this->create_text(parent, icon, text, LV_MENU_ITEM_BUILDER_VARIANT_2);
                        PalcomTextarea textarea;
                        textarea.create(obj);
                        textarea.setSize(120, 40);
			textarea.setValidChars("0123456789");
                        if(value != NULL){
                                textarea.setText((const char *)value);
                        }


                        return obj;

                }

		void createGeneralPage(void){
			String userName = (const char *)configData.user_name;
			String lockTimer = to_string(configData.lock_timer).c_str();
			sprintf((char *)compBuffer, "%d", configData.lock_timer);
			generalPage.create(this->getObject(), NULL);
                        generalPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
                        generalPage.addSeparator();
                        generalPage.section.create(generalPage.getObject());
                        lv_obj_t * obj = this->create_text(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "Name", LV_MENU_ITEM_BUILDER_VARIANT_2);
			PalcomTextarea textarea;
                        textarea.create(obj);
                        textarea.setSize(120, 40);
			textarea.setParamCallback(&callback_processUsername, (void *)userName.c_str(), LV_EVENT_DEFOCUSED);
                        textarea.setText((const char *)configData.user_name);
			this->execute();

			obj = this->create_text(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "Lock Timer", LV_MENU_ITEM_BUILDER_VARIANT_2);
                        textarea.create(obj);
                        textarea.setSize(120, 40);
                        textarea.setParamCallback(&callback_processLockTimer, (void *)&configData.lock_timer, LV_EVENT_DEFOCUSED);
                        textarea.setText(lockTimer.c_str());
			this->execute();
			

                        this->create_slider(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "Brightness", 1, 255, configData.screen_brightness);
			this->execute();
		}

		void createSecurityPage(void){
			securityPage.create(this->getObject(), NULL);
                        securityPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
                        securityPage.addSeparator();
                        securityPage.section.create(securityPage.getObject());
                        this->create_switch(securityPage.section.getObject(), LV_SYMBOL_SETTINGS, "PMode", authData.paranoia_mode == 1 ? true : false);
			this->execute();
                        this->create_button(securityPage.section.getObject(), LV_SYMBOL_SETTINGS, "Change Passcode", &callback_newPasscode);
			this->execute();
		}

		void createFactoryResetPage(void){
			resetPage.create(this->getObject(), NULL);
                        resetPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
                        resetPage.addSeparator();
                        resetPage.section.create(resetPage.getObject());
                        this->create_button(resetPage.section.getObject(), LV_SYMBOL_SETTINGS, "Factory Reset", &callback_factoryReset);
			this->execute();
		}
	public:
		palcom_config_t configData;
		palcom_auth_t authData;

		bool newPinMode(void){
			return settingsMenu_newPin;
		}
		void reset(void){
			settingsMenu_exit = false;
			settingsMenu_newPin = false;
			settingsMenu_updateTimer = false;
			settingsMenu_updateBrightness = false;
			
			uint8_t *configData_ptr = (uint8_t*)&configData;
                	uint8_t *authData_ptr = (uint8_t*)&authData;
			
			for(int i=0; i<sizeof(palcom_config_t); i++){
				configData_ptr[i] = 0x00;
			}

			for(int i=0; i<sizeof(palcom_auth_t); i++){
				authData_ptr[i] = 0x00;
			}
		}

		void fetchConfigData(void){
			PalcomFS pfs;
                        palcom_config_t *configData_ptr = pfs.getConfigData();
			configData.lock_timer = configData_ptr->lock_timer;
			configData.screen_brightness = configData_ptr->screen_brightness;
			for(int i=0; i<20; i++){
				configData.user_name[i] = configData_ptr->user_name[i];
			}
		}

		void fetchAuthData(void){
			PalcomPartition pp;
			if(!pp.fetchPartitionByName("app1")){
				throw CoreException("SettingsMenu::fetchAuthData() - Failed to find auth data storage.", 0x01);
			}

			pp.readAuthData((const esp_partition_t *)pp.partition, &authData);
			pp.freePartitions();
		}

		bool updateTimer(void){
			if(settingsMenu_updateTimer){
				settingsMenu_updateTimer = false;
				return true;
			}
			return false;
		}

		bool updateBrightness(void){
			if(settingsMenu_updateBrightness){
				settingsMenu_updateBrightness = false;
				return true;
			}
			return false;
		}

		void make(lv_obj_t *parent, void (*exitFunc)(lv_event_t *)){
			this->create(parent);
			this->execute();

	    		lv_color_t bg_color = this->getStyleBgColor();
	    		if(this->colors.getBrightness(bg_color) > 127) {
		    		this->setStyleBgColor(this->colors.darken(this->getStyleBgColor(), 10));
	    		}else {
		    		this->setStyleBgColor(this->colors.darken(this->getStyleBgColor(), 50));
	    		}

	    		this->enableRootBackButton(true);
			this->setParamCallback(exitFunc, (void*)this->getObject(), LV_EVENT_CLICKED);
			this->fullScreen();
	    		this->center();
			this->execute();

		    	lv_obj_t * cont;
			PalcomSection section;

			/* Create General Page */
			this->createGeneralPage();
			this->execute();

			/* Create Security Page */
			this->createSecurityPage();
			this->execute();

			/* Create Relay Page */
			relayPage.create(this->getObject(), NULL);
	    		relayPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
	    		relayPage.addSeparator();
	    		relayPage.section.create(relayPage.getObject());
	    		this->create_slider(relayPage.section.getObject(), LV_SYMBOL_SETTINGS, "Relay IP", 0, 150, 100);
	    		this->create_slider(relayPage.section.getObject(), LV_SYMBOL_SETTINGS, "Relay Port", 0, 150, 100);
			this->execute();

			/* Create factory reset page */
			this->createFactoryResetPage();
			this->execute();

	    		/*Create a root page*/
			rootPage.create(this->getObject(), "Settings");
			rootPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
			this->execute();
	    		
			rootPage.section.create(rootPage.getObject());
	    		cont = this->create_text(rootPage.section.getObject(), LV_SYMBOL_SETTINGS, "General", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, generalPage.getObject());
	    		
			cont = this->create_text(rootPage.section.getObject(), LV_SYMBOL_SETTINGS, "Security", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, securityPage.getObject());
	    		
			cont = this->create_text(rootPage.section.getObject(), LV_SYMBOL_SETTINGS, "Relay", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, relayPage.getObject());

	    		this->create_text(rootPage.getObject(), NULL, "Admin", LV_MENU_ITEM_BUILDER_VARIANT_1);
	    		
			rootPage.section.create(rootPage.getObject());
    			cont = this->create_text(rootPage.section.getObject(), NULL, "Factory Reset", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, resetPage.getObject());

    			this->setSidebarPage(rootPage.getObject());
	
    			lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(this->getObject()), 0), 0), LV_EVENT_CLICKED,NULL);
		}
};
