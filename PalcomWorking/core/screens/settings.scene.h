int settings_context = CONTEXT_SETTINGS;

class PalcomSettingsMenu : public PalcomScreen{
  	private:
		SettingsMenu menu;
    		
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

		static void Settingsmenu_wiFiMenu(lv_event_t *e){
			if(lv_event_get_code(e) != LV_EVENT_CLICKED){
                                return;
                        }

			settings_context = CONTEXT_WIFI;
			/*WiFi.mode(WIFI_STA);
    			WiFi.disconnect();
    			delay(100);

			int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();*/
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

	public:
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

			menu.make(screen, &backButton);
			this->execute();
    		}

    		void resetPage(void){
      			this->setBuildRequired(true);
      			settings_context = CONTEXT_SETTINGS;
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

      			return settings_context;
    		}
}settingsMenu;
