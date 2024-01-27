int mainMenu_contextControl = 0;

class PalcomMainMenu : public PalcomScreen{
  	private:
		const lv_img_dsc_t *img_src[1] = {&palcomLogo};
		const lv_img_dsc_t *img_lockIcon[1] = {&LockIcon};

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
    			this->setScreenScrollDirection(LV_DIR_VER);
    			this->setBgImage(img_src);
			this->execute();

    			PalcomLabel pLabel;

    			// Create Settings Button
    			PalcomImageButton settings;
    			settings.create(screen);
			defaultImagebuttonStyle.initStyle();
                        settings.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			settings.setButtonImage(NULL, &SettingsIcon, NULL);
    			settings.setSizeRaw(50, 50);
    			settings.setSimpleCallback(Mainmenu_handleSettingsButton);
    			settings.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 0,  50);
			this->execute();

			// Create Messages Button
    			PalcomImageButton messages;
    			messages.create(screen);
			defaultButtonStyle.initStyle();
                        messages.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			messages.setButtonImage(NULL, &MessageIcon, NULL);
    			messages.setSizeRaw(100, 100);
    			pLabel.create(messages.getObj());
    			pLabel.setText("Messages");
    			pLabel.center();
    			messages.setLabel(pLabel);
    			messages.setSimpleCallback(Mainmenu_handleMessagesButton);
    			messages.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, -20,  100+70);
			this->execute();

			// Create Keyshare Button
                        PalcomImageButton keyshare;
                        keyshare.create(screen);
                        defaultButtonStyle.initStyle();
                        keyshare.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        keyshare.setButtonImage(NULL, &MessageIcon, NULL);
                        keyshare.setSizeRaw(100, 100);
                        pLabel.create(keyshare.getObj());
                        pLabel.setText("Keyshare");
                        pLabel.center();
                        keyshare.setLabel(pLabel);
                        keyshare.setSimpleCallback(Mainmenu_handleKeyshareButton);
                        keyshare.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, -20+100+10,  100+70);
                        this->execute();

			// Create Encrypted Button
                        PalcomImageButton encrypted;
                        encrypted.create(screen);
                        defaultButtonStyle.initStyle();
                        encrypted.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        encrypted.setButtonImage(NULL, &MessageIcon, NULL);
                        encrypted.setSizeRaw(100, 100);
                        pLabel.create(keyshare.getObj());
                        pLabel.setText("Encrypted");
                        pLabel.center();
                        encrypted.setLabel(pLabel);
                        encrypted.setSimpleCallback(Mainmenu_handleEncryptedMessagesButton);
                        encrypted.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, -20+125, 55);
                        this->execute();

    			// Create Logout Button
    			PalcomImageButton logout;
    			logout.create(screen);
			defaultImagebuttonStyle.initStyle();
                        logout.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			logout.setButtonImage(NULL, &LockIcon, NULL);
			logout.setSizeRaw(50, 50);
    			logout.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0, 50);
    			logout.setSimpleCallback(Mainmenu_handleLogoutButton);
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
