int settings_context = CONTEXT_SETTINGS;

class PalcomSettingsMenu : public PalcomScreen{
  	private:
		SettingsMenu menu;
		
		PalcomPinpad pinpad;
		int pinCtx = 0;

		bool newPinMode = false;
		bool pinAuthed = false;
    		
		static void Settingsmenu_yesPopup(lv_event_t *e){
                        if(lv_event_get_code(e) != LV_EVENT_CLICKED){
                                return;
                        }
			settings_context = -1;
                        PalcomFS pfs;
                        pfs.rm("/");
                        PalcomScreenError = 0;
                }


		static void backButton(lv_event_t *e){
                        //if(lv_event_get_code(e) == LV_EVENT_RELEASED){
                                lv_obj_t *o = (lv_obj_t *)lv_event_get_target(e);
                                lv_obj_t *m = (lv_obj_t *)lv_event_get_user_data(e);
				if(lv_menu_back_button_is_root(m, o)) {
                                	settings_context = CONTEXT_MAINMENU;
				}
                        //}
                }

		void drawPinpad(lv_obj_t *parent){
			this->pinpad.create(parent, "Enter Passcode");
		}

		void processNewPinMode(void){
			if(pinpad.codeReady() && !pinpad.transferReady()){
                                if(this->pinCtx == 0){
                                        pinpad.transferResult();
                                        PalcomHash phash;
                                        phash.useSHA256();
                                        phash.run((unsigned char *)pinpad.entryBuffer, pinpad.entryBufferCount);
                                        String result = phash.getResultStr();
                                        if(phash.getResultSize() > 32)
                                                throw CoreException("PalcomSetup::run() - Invalid hash size.", 0x01);

                                        for(int i=0; i<32; i++){
                                                if(result[i] != menu.authData.pin_hash[i]){
                                                        pinpad.clear();
                                                        Serial.printf("Invalid passcode.\n");
                                                        pinpad.setTitleText("Invalid Passcode");
                                                        break;
                                                }else{
                                                        pinpad.clear();
                                                        pinpad.setTitleText("Enter new passcode");
                                                        errorMsg = "";
                                                        this->pinCtx = 1;
                                                }
                                        }
                                }else if(this->pinCtx == 1){
					pinpad.transferResult();
                                        pinpad.setTitleText("Confirm Passcode");
                                        this->pinCtx = 2;
                                }else{
                                        return;
                                }
                        }else if(pinpad.codeReady() && pinpad.transferReady()){
                                if(pinpad.compResults()){
                                        PalcomHash phash;
                                        phash.useSHA256();
                                        phash.run((unsigned char *)pinpad.entryBuffer, pinpad.entryBufferCount);
                                        String result = phash.getResultStr();
                                        if(phash.getResultSize() > 32)
                                                throw CoreException("PalcomSettings::run() - Invalid hash size.", 0x01);

                                        menu.fetchAuthData();
                                        for(int i=0; i<32; i++){
                                                menu.authData.pin_hash[i] = result[i];
                                        }

                                        PalcomPartition pp;
                                        if(!pp.fetchPartitionByName("app1")){
                                                throw CoreException("PalcomSettings::run() - Failed to find security partition.", 0x02);
                                        }

                                        pp.writeAuthData((const esp_partition_t *)pp.partition, menu.authData);
                                        pp.freePartitions();

					this->pinCtx = 0;
                                        newPinMode = false;
                                        pinpad.clear();
                                        menu.reset();
                                        this->setBuildRequired(true);
                                        this->destroy();

                                }else{
                                        pinpad.clear();
                                        pinpad.setTitleText("Passcodes invalid.");
                                        this->pinCtx = 1;
                                }
                        }
		}

	public:
		String errorMsg = "";

    		void generateObjects(void){
			// Establish screen descriptor
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);


			if(this->newPinMode){
				this->drawPinpad(screen);
			}else{	
				this->menu.make(screen, &backButton);
			}
			this->execute();
    		}

    		void resetPage(void){
      			this->setBuildRequired(true);
      			settings_context = CONTEXT_SETTINGS;
      			this->globalDestroy();
      			this->destroy();
			this->clearScreenError();
			this->newPinMode = false;
			this->menu.reset();
    		}

    		int run(void){
      			if(this->getBuildRequired()){
				menu.fetchConfigData();
				menu.fetchAuthData();
        			this->setBuildRequired(false);
        			settings_context = 2;
        			this->load();
      			}

			this->execute();

			if(menu.newPinMode() && !newPinMode){
				this->setBuildRequired(true);
				this->destroy();
				newPinMode = true;
				errorMsg = "";
				return settings_context;
			}

			if(newPinMode){
				this->processNewPinMode();
			}
			
      			return settings_context;
    		}
}settingsMenu;
