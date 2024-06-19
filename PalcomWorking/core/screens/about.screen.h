bool exitPalcomAboutScreen = false;
class PalcomAboutScreen : public PalcomScreen{
	private:
		static void exitAboutButton(lv_event_t *e){
			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
				exitPalcomAboutScreen = true;	
			}	
		}
	public:
		PalcomAboutScreen(void){
			exitPalcomAboutScreen = false;
		}
		void generateObjects(void){
			PalcomPartition pp;
			pp.fetchPartitionByName("app1");
			palcom_auth_t authData;
			pp.readAuthData((const esp_partition_t *)pp.partition, &authData);
			pp.freePartitions();

			sprintf((char *)fileData, "%d (%d%d%d) %d%d%d-%d%d%d%d", 
					authData.phone_number[0],
					authData.phone_number[1],
					authData.phone_number[2],
					authData.phone_number[3],
					authData.phone_number[4],
					authData.phone_number[5],
					authData.phone_number[6],
					authData.phone_number[7],
					authData.phone_number[8],
					authData.phone_number[9],
					authData.phone_number[10]
				);

			lv_obj_t *screen = this->getScreen();
			if(screen == NULL){
				this->globalDestroy();
				this->create();
				screen = this->getScreen();
			}

			this->setFullScreen();
			this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
			this->execute();


			PalcomLabel titleLabel;
			titleLabel.create(screen);
			titleLabel.setLongMode(LV_LABEL_LONG_SCROLL);
                        titleLabel.setWidth(300);
			titleLabel.setText("About This Device");
			titleLabel.setAlignment(LV_ALIGN_OUT_TOP_MID, 80, 20);
			this->execute();


			String info = "Version : ";
			info += SOFTWARE_VERSION;
			info += "\n\nNumber : ";
			info += (const char *)fileData;
			info += "\n\nSecured LoRa Texting Device\nmade By BestosoTech.";
			PalcomLabel infoLabel;
			infoLabel.create(screen);
			infoLabel.setText(info.c_str());
			infoLabel.setAlignment(LV_ALIGN_OUT_TOP_MID, 40, 80);
			this->execute();

			PalcomButton backButton;
			PalcomLabel backLabel;
			style_pinpadButton.initStyle();

			backButton.create(screen);
			backLabel.create(backButton.getObject());
			backLabel.setText("Done");
			backLabel.center();
			backButton.setLabel(backLabel);
			backButton.setDefaultStyle(style_pinpadButton.getStyle());
			backButton.setPressedStyle(style_pinpadButton.getPressedStyle());
			backButton.setSize(50, 15);
			backButton.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 80, 190);
			backButton.setSimpleCallback(&exitAboutButton);
		}

		void resetPage(void){
			this->setBuildRequired(true);
			this->destroy();
			exitPalcomAboutScreen = false;
		}

		int run(void){
			if(this->getBuildRequired()){
				this->setBuildRequired(false);
				this->load();
			}
			this->execute();

			if(exitPalcomAboutScreen){
				this->destroy();
				this->setBuildRequired(true);
				return CONTEXT_MAINMENU;
			}
			return CONTEXT_ABOUT;
		}
}aboutScreen;
