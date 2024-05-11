int wifi_context = 0;
bool wifi_scanForNetworks = false;
#define WIFIMENU_SIGNALBUF_MAXLEN 20

class PalcomWiFiMenu : public PalcomScreen{
  	private:
		wifisignal_t wifiSignals[WIFIMENU_SIGNALBUF_MAXLEN];
		size_t wifiSignalCount = 0;
		int alternateMenuReturn = -1;

    		static void WifiMenu_handleBackButton(lv_event_t *e){
      			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;
      			wifi_context = 1;
    		}

		static void WifiMenu_handleScanForNetworks(lv_event_t *e){
			if(lv_event_get_code(e) != LV_EVENT_CLICKED){
				return;
			}

			wifi_scanForNetworks = true;
		}
    
	public:
		bool validAlternateReturn(void){
			return (alternateMenuReturn >= 0);
		}

		void setAlternateReturn(int value){
			alternateMenuReturn = value;
		}
    		void generateObjects(void){
      			PalcomLabel pLabel;
      			PalcomButton settings;
      			PalcomFS pfs;
			PalcomPopupMessage palpop;

			// Establish screen descriptor
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);

			int err = this->getScreenError();

			// WiFi Signal Connect
			PalcomButton scanButton;
			scanButton.create(screen);
			defaultButtonStyle.init();
			scanButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
			scanButton.setSizeRaw(75, 50);
			scanButton.setSimpleCallback(WifiMenu_handleScanForNetworks);
			scanButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 10, 50);
			pLabel.create(scanButton.getObj());
				pLabel.setText("Scan");
				pLabel.center();
			scanButton.setLabel(pLabel);
			this->execute();
			if(wifi_scanForNetworks){
				Serial.printf("Scanning network...\n");
				WiFiSignal ws;
				wifiSignalCount = ws.scanForNetworks(wifiSignals, WIFIMENU_SIGNALBUF_MAXLEN);
				wifi_scanForNetworks = false;
			}
			// Display Detected Networks
			if(wifiSignalCount > 0){
				Serial.printf("===DEBUG WIFI OPTIONS===\n");
				int buttonW = 225;
				int buttonH = 50;
				for(int i=0; i<wifiSignalCount; i++){
					PalcomButton signalButton;
                        		signalButton.create(screen);
                        		defaultButtonStyle.init();
                        		signalButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        		signalButton.setSizeRaw(buttonW, buttonH);
                        		//scanButton.setSimpleCallback(WifiMenu_handleScanForNetworks);
                        		signalButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 20, 110+((buttonH+10)*i));
                        		pLabel.create(signalButton.getObj());
                        		        pLabel.setText(wifiSignals[i].ssid.c_str());
                        		        pLabel.center();
                        		signalButton.setLabel(pLabel);
                        		this->execute();
					Serial.printf("\t%s %ld %d %s\n", wifiSignals[i].ssid.c_str(), wifiSignals[i].rssi, wifiSignals[i].channel, wifiSignals[i].cipher_alg.c_str());
				}
			}

			// Create Back Button.
      			PalcomImageButton backButton;
      			backButton.create(screen);
			defaultImagebuttonStyle.init();
			backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
			backButton.setButtonImage(NULL, &BackIcon, NULL);
      			backButton.setSizeRaw(50, 50);
      			backButton.setSimpleCallback(WifiMenu_handleBackButton);
      			backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
			this->execute();

      			// Callsign input
			/*PalcomTextInput callSignInput;
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
*/
		/*	// WiFi Settings Link
			PalcomButton wiFiSettings;
			wiFiSettings.create(screen);
			defaultButtonStyle.initStyle();
			wiFiSettings.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
			wiFiSettings.setSizeRaw(100, 100);
			pLabel.create(wiFiSettings.getObj());
				pLabel.setText("WiFi");
				pLabel.center();
			wiFiSettings.setLabel(pLabel);
			wiFiSettings.setSimpleCallback(Settingsmenu_wiFiMenu);
			wiFiSettings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
			this->execute();*/
    		}

    		void resetPage(void){
      			this->setBuildRequired(true);
      			wifi_context = 0;
			alternateMenuReturn = -1;
      			this->globalDestroy();
      			this->destroy();
			this->clearScreenError();
    		}

    		int run(void){
			if(wifi_scanForNetworks){
				this->setBuildRequired(true);
			}
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			wifi_context = CONTEXT_WIFI;
        			this->load();
      			}


			this->execute();

			if(this->isScreenError()){
				Serial.printf("Detected a screen error : %d\n", PalcomScreenError);
      				this->globalDestroy();
      				this->destroy();
				this->setBuildRequired(true);
                        }

			if(this->validAlternateReturn()){
				if(wifi_context != CONTEXT_WIFI){
					return alternateMenuReturn;
				}
			}
      			return wifi_context;
    		}
}wifiMenu;
