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
      			settings_context = -1;
      			PalcomFS pfs;
      			pfs.rm("/");
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
			PalcomScreenError = 2;
		}
		
  
	public:
    		void generateObjects(void){
			Serial.printf("Generating settings page.\n");
      			PalcomLabel pLabel;
			PalcomPopupMessage popupDebug;
      			PalcomImageButton settings;
      			PalcomFS pfs;

      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);
			this->execute();

			int err = this->getScreenError();
			if(err == 1){  // Updated Callsign.
				popupDebug.setTitle("Settings Changed.\n");
				popupDebug.setMessage("Callsign has been changed.");
				popupDebug.setCloseHandler(Settingsmenu_closePopup);
				popupDebug.create(screen);
				defaultPopupStyle.initStyle();
				popupDebug.setStyle(defaultPopupStyle.getStyle());
				this->execute();
				return;
			}

			// Create Back Button.
      			settings.create(screen);
			defaultImagebuttonStyle.init();
			settings.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			settings.setButtonImage(NULL, &BackIcon, NULL);
      			settings.setSizeRaw(50, 50);
      			settings.setSimpleCallback(Settingsmenu_handleBackButton);
      			settings.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
			this->execute();

      			// Callsign input
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
      			pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 90);
      			pLabel.setText("Callsign: ");
			this->execute();
      			PalcomTextarea callsign;
      			callsign.createGlobal(screen, 1);
      			callsign.setCursorClickPos(false);
      			callsign.setTextSelection(false);
      			callsign.setSize(150, 23);
      			callsign.setText(pfs.getCallsign());
      			callsign.setMaxLength(18);
      			callsign.setOneLine(true);
      			callsign.setAlignment(LV_ALIGN_TOP_MID, 15, 83);
			this->execute();
      			settings.create(screen);
      			settings.setSize(20, 12);
      			pLabel.create(settings.getObj());
      			pLabel.setText("Update");
      			pLabel.center();
      			settings.setLabel(pLabel);
      			settings.setSimpleCallback(Settingsmenu_setCallsign);
      			settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 250,  75);
			this->execute();

			// Username Input

			// Password Input
			
			// Screen Sleep Input

      			// factory reset button
      			settings.create(screen);
      			settings.setSize(40, 25);
      			pLabel.create(settings.getObj());
      			pLabel.setText("reset");
      			pLabel.center();
      			settings.setLabel(pLabel);
      			settings.setSimpleCallback(Settingsmenu_handleResetButton);
      			settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  100);
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
      				this->globalDestroy();
      				this->destroy();
				this->setBuildRequired(true);
                        }

      			return settings_context;
    		}
}settingsMenu;
