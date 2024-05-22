int mainMenu_contextControl = 0;

class PalcomMainMenu : public PalcomScreen{
  	private:
		const lv_img_dsc_t *img_src[1] = {&palcomLogo};
		const lv_img_dsc_t *img_lockIcon[1] = {&LockIcon};

		static void menuButtonHandler(lv_event_t *e){
			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
				uint8_t *val = (uint8_t *)lv_event_get_user_data(e);
                                sprintf((char *)fileData, "%d", val);

				int ctrl = fileData[0] == '0' ? 0 : fileData[0] == '1' ? 1 : fileData[0] == '2' ? 2 : 3;
				switch(ctrl){
					case 0: // Contacts
						Serial.printf("Clicked 'Contacts' button!\n");
						break;
					case 1: // Settings
						Serial.printf("Clicked 'Settings' button!\n");
						break;
					case 2: // About
						Serial.printf("Clicked 'About' button!\n");
						break;
					case 3: // Lock Screen
						Serial.printf("Clicked 'Lock' button!\n");
						break;
					default:
						Serial.printf("Invalid menu option.\n");
				}
			}
		}

    		static void Mainmenu_handleLogoutButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      			mainMenu_contextControl = 1;
    		}

    		static void Mainmenu_handleSettingsButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      			mainMenu_contextControl = 2;
    		}

    		static void Mainmenu_handleMessagesButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      			mainMenu_contextControl = 3;
    		}

		static void Mainmenu_handleEncryptedMessagesButton(lv_event_t *e){
                        if (lv_event_get_code(e) != LV_EVENT_CLICKED)
                                return;
			Serial.printf("Switching to encrypted page.\n");
                        mainMenu_contextControl = 4;
                }

		static void Mainmenu_handleKeyshareButton(lv_event_t *e){
			if(lv_event_get_code(e) != LV_EVENT_CLICKED)
				return;
			mainMenu_contextControl = 5;
		}

		void createContactsButton(lv_obj_t *parent){
			PalcomLabel contactLabel;
                        PalcomButton contactButton;
                        style_pinpadButton.initStyle();

                        contactButton.create(parent);
                        contactButton.setSize(98, 20);
                        contactButton.setDefaultStyle(style_pinpadButton.getStyle());
                        contactButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        contactButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, 1, 20);
                        contactButton.setValuedCallback(&menuButtonHandler, (uint8_t *)0);
                        contactLabel.create(contactButton.getObj());
                        contactLabel.setText("Contacts");
                        contactLabel.center();
                        contactButton.setLabel(contactLabel);
		}

		void createSettingsButton(lv_obj_t *parent){
			PalcomLabel settingLabel;
			PalcomButton settingButton;
			style_pinpadButton.initStyle();

			settingButton.create(parent);
			settingLabel.create(settingButton.getObj());
			settingLabel.setText("Settings");
			settingLabel.center();
			settingButton.setLabel(settingLabel);
			settingButton.setDefaultStyle(style_pinpadButton.getStyle());
			settingButton.setPressedStyle(style_pinpadButton.getPressedStyle());
			settingButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, 1, 80);
			settingButton.setValuedCallback(&menuButtonHandler, (uint8_t*)1);
			settingButton.setSize(98, 20);
		}

		void createAboutButton(lv_obj_t *parent){
                        PalcomLabel settingLabel;
                        PalcomButton settingButton;
                        style_pinpadButton.initStyle();

                        settingButton.create(parent);
                        settingLabel.create(settingButton.getObj());
                        settingLabel.setText("About");
                        settingLabel.center();
                        settingButton.setLabel(settingLabel);
                        settingButton.setDefaultStyle(style_pinpadButton.getStyle());
                        settingButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        settingButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, 1, 80+60);
                        settingButton.setValuedCallback(&menuButtonHandler, (uint8_t*)2);
                        settingButton.setSize(98/2-5, 20);
                }

		void createLockButton(lv_obj_t *parent){
                        PalcomLabel settingLabel;
                        PalcomButton settingButton;
                        style_pinpadButton.initStyle();

                        settingButton.create(parent);
                        settingLabel.create(settingButton.getObj());
                        settingLabel.setText("Lock");
                        settingLabel.center();
                        settingButton.setLabel(settingLabel);
                        settingButton.setDefaultStyle(style_pinpadButton.getStyle());
                        settingButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        settingButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, 175, 80+60);
                        settingButton.setValuedCallback(&menuButtonHandler, (uint8_t*)3);
                        settingButton.setSize(98/2-5, 20);
                }
  	
	public:
  		void generateObjects(void){
    			// Configure Screen
    			lv_obj_t *screen = this->getScreen();
    			if(screen == NULL){
      				this->globalDestroy();
      				this->create();
      				screen = this->getScreen();
    			}
    			this->setFullScreen();
    			this->setScrollMode(LV_SCROLLBAR_MODE_OFF);
			this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
			this->execute();
			
			this->createContactsButton(screen);
			this->execute();

			this->createSettingsButton(screen);
			this->execute();

			this->createAboutButton(screen);
			this->execute();

			this->createLockButton(screen);
			this->execute();
			
  		}

  		void resetPage(){
  			this->setBuildRequired(true);
    			mainMenu_contextControl = 0;
    			this->globalDestroy();
    			this->destroy();
			this->clearScreenError();
  		}

  		int run(){
    			if(this->getBuildRequired()){
      				this->setBuildRequired(false);
      				mainMenu_contextControl = 0;
      				this->load();
    			}
    
   			this->execute();

    			if(mainMenu_contextControl == 1){
      				this->globalDestroy();
      				this->destroy();
      				this->setBuildRequired(true);
      				return 0; // logout
    			}

    			if(mainMenu_contextControl == 2){
      				this->globalDestroy();
      				this->destroy();
      				this->setBuildRequired(true);
      				return 2; // Settings
    			}

    			if(mainMenu_contextControl == 3){
      				this->globalDestroy();
      				this->destroy();
      				this->setBuildRequired(true);
      				return 3; // Messages
    			}

			if(mainMenu_contextControl == 4){
                                this->globalDestroy();
                                this->destroy();
                                this->setBuildRequired(true);
                                return 4; // Encrypted
                        }

			if(mainMenu_contextControl == 5){
				this->globalDestroy();
                                this->destroy();
                                this->setBuildRequired(true);
                                return 5; // Keyshare
			}
   			return 1;
  		}
}mainMenu;
