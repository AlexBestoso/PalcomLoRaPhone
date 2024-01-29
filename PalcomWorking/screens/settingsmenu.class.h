int settings_context = 0;

class PalcomSettingsMenu : public PalcomScreen{
  	private:
    		static void Settingsmenu_handleBackButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      			settings_context = 1;
    		}
    
		static void Settingsmenu_handleResetButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
			PalcomScreenError = 2;
    		}
    
		static void Settingsmenu_setCallsign(lv_event_t *e){
      			if(lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      
      			PalcomFS pfs;
      			PalcomTextarea callsign;
      			callsign.loadGlobal(1);

      			pfs.setCallsign(callsign.getText());
			PalcomScreenError = 1;
    		}

		static void Settingsmenu_closePopup(lv_event_t *e){
			if(lv_event_get_code(e) != LV_EVENT_CLICKED){
				return;
			}
			PalcomScreenError = 999;
		}

		static void Settingsmenu_yesPopup(lv_event_t *e){
                        if(lv_event_get_code(e) != LV_EVENT_CLICKED){
                                return;
                        }
			settings_context = -1;
                        PalcomFS pfs;
                        pfs.rm("/");
                        PalcomScreenError = 0;
                }

		static void Settingsmenu_noPopup(lv_event_t *e){
                        if(lv_event_get_code(e) != LV_EVENT_CLICKED){
                                return;
                        }
                        PalcomScreenError = 999;
                }
		
  
	public:
    		void generateObjects(void){
      			PalcomLabel pLabel;
      			PalcomButton settings;
      			PalcomFS pfs;
			PalcomPopupMessage palpop;

      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);

			int err = this->getScreenError();
			if(err == 1){  // Updated Callsign.
				palpop.setPopupType(1);
				palpop.setTitle("Settings Changed.\n");
				palpop.setMessage("Callsign has been changed.");
				palpop.setCloseHandler(Settingsmenu_closePopup);
				palpop.create(screen);
				defaultPopupStyle.initStyle();
				palpop.setStyle(defaultPopupStyle.getStyle());
				this->execute();
				return;
			}
			
			if(err == 2){ // Factory Reset
				palpop.setPopupType(2);
				palpop.setTitle("Are You Sure You Want To Do This?");
				palpop.setMessage("You're about to factory reset your device.\nAre you sure you want to?");
				palpop.setYesHandler(Settingsmenu_yesPopup);
				palpop.setNoHandler(Settingsmenu_noPopup);
				palpop.create(screen);
				defaultPopupStyle.initStyle();
				palpop.setStyle(defaultPopupStyle.getStyle());
				this->execute();
				return;
			}


			// Create Back Button.
      			PalcomImageButton backButton;;
      			backButton.create(screen);
			defaultImagebuttonStyle.init();
			backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			backButton.setButtonImage(NULL, &BackIcon, NULL);
      			backButton.setSizeRaw(50, 50);
      			backButton.setSimpleCallback(Settingsmenu_handleBackButton);
      			backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
			this->execute();

      			// Callsign input
			PalcomTextInput callSignInput;
			callSignInput.setInputWidth(270+50);
			callSignInput.setInputHeight(50);
			callSignInput.setInputY(50+10);
			
			callSignInput.setLabelText("Callsign:");

			callSignInput.setTextareaId(1);
			callSignInput.setTextareaWidth(140);
			callSignInput.setTextareaHeight(18);
			callSignInput.setTextareaX(70);
			callSignInput.setTextareaY(-8);
			callSignInput.setDefaultText(pfs.getCallsign());
			callSignInput.setButtonText("Submit");
			callSignInput.setSubmitHandlerr(Settingsmenu_setCallsign);

			callSignInput.create(screen);

			defaultTextInputStyle.initStyle();
			callSignInput.setStyle(defaultTextInputStyle.getStyle());
			this->execute();

			// Username Input

			// Password Input
			
			// Screen Sleep Input

      			// factory reset button
			PalcomButton factoryReset;
      			factoryReset.create(screen);
			defaultButtonStyle.initStyle();
			factoryReset.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
      			factoryReset.setSizeRaw(50, 50);
      			pLabel.create(factoryReset.getObj());
      			pLabel.setText("reset");
      			pLabel.center();
      			factoryReset.setLabel(pLabel);
      			factoryReset.setSimpleCallback(Settingsmenu_handleResetButton);
      			factoryReset.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_RIGHT, 275,  160);
			this->execute();
    		}

    		void resetPage(void){
      			this->setBuildRequired(true);
      			settings_context = 0;
      			this->globalDestroy();
      			this->destroy();
			this->clearScreenError();
    		}

    		int run(void){
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			settings_context = 2;
        			this->load();
      			}

			this->execute();

			if(this->isScreenError()){
				Serial.printf("Detected a screen error : %d\n", PalcomScreenError);
      				this->globalDestroy();
      				this->destroy();
				this->setBuildRequired(true);
                        }

      			return settings_context;
    		}
}settingsMenu;
