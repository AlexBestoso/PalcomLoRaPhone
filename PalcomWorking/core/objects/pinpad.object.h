#define NUMPAD_ENTRY_SIZE 6
static char numPadEntry[NUMPAD_ENTRY_SIZE];
static int numPadEntryCount;
PalcomLabel numpadTitleLabel;
class PalcomPinpad : public PalcomObject{
	private:
		int lockoutTime = 0;
		int timer = 0;

		static void numDeleteHandler(lv_event_t *e){
			PalcomEvent event(e);
			if(event.getCode() == LV_EVENT_RELEASED){
				if(numPadEntryCount <= 0){
					String msg = (const char *)event.getUserData();
					if(msg == "")
						numpadTitleLabel.setText("Enter Pin");
					else
						numpadTitleLabel.setText(msg.c_str());
					return;
				}
				numPadEntryCount--;
				String code = "";
                                for(int i=0; i<numPadEntryCount; i++)
                                        code += '*';
                                numpadTitleLabel.setText(code.c_str());
			}
		}
		static void numButtonHandler(lv_event_t* e){
			if(lv_event_get_code(e) == LV_EVENT_RELEASED){
				if(numPadEntryCount >= 6)
					return;
				uint8_t *val = (uint8_t *)lv_event_get_user_data(e);
				sprintf((char *)fileData, "%d", val);
				numPadEntry[numPadEntryCount] = fileData[0];
				numPadEntryCount++;

				String code = "";
				for(int i=0; i<numPadEntryCount; i++)
					code += '*';
				numpadTitleLabel.setText(code.c_str());
			}
		}

		void createNumButton(lv_obj_t *parent, String buttonText, int x, int y){
			PalcomLabel numLabel;
                        PalcomButton numButton;

                        numButton.create(parent);
                        numButton.setSize(20, 20);
                        style_pinpadButton.initStyle();
                        numButton.setDefaultStyle(style_pinpadButton.getStyle());
                        numButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        numButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, x, y);
			numButton.setValuedCallback(&numButtonHandler, (uint8_t *)buttonText.toInt());
                        numLabel.create(numButton.getObj());
                        numLabel.setText(buttonText.c_str());
                        numLabel.center();
                        numButton.setLabel(numLabel);
		}

		void createDeleteButton(lv_obj_t *parent, int x, int y){
			PalcomLabel numLabel;
                        PalcomButton numButton;

                        numButton.create(parent);
                        numButton.setSize(20*2, 20);
                        style_pinpadButton.initStyle();
                        numButton.setDefaultStyle(style_pinpadButton.getStyle());
                        numButton.setPressedStyle(style_pinpadButton.getPressedStyle());
                        numButton.setAlignment(LV_ALIGN_OUT_TOP_LEFT, x, y);
                        numButton.setParamCallback(&numDeleteHandler, (void *)errorMsg.c_str());
                        numLabel.create(numButton.getObj());
                        numLabel.setText("<");
                        numLabel.center();
                        numButton.setLabel(numLabel);
		}
	public:
		char entryBuffer[NUMPAD_ENTRY_SIZE];
		int entryBufferCount = 0;
		String errorMsg = "";

		PalcomPinpad(void){
			clearEntry();
		}

		uint8_t getFailCount(void){
			PalcomPartition pp;
			palcom_auth_t authData;
			
			pp.fetchPartitionByName("app1");
			pp.readAuthData((const esp_partition_t *)pp.partition, &authData);
			pp.freePartitions();

			return authData.fail_count;
		}

		void clearFailCount(void){
			PalcomPartition pp;
			palcom_auth_t authData;
			pp.fetchPartitionByName("app1");
			pp.readAuthData((const esp_partition_t *)pp.partition, &authData);
			authData.fail_count = 0;
			pp.writeAuthData((const esp_partition_t *)pp.partition, authData);
			pp.freePartitions();
		}

		void incFailCount(void){
			PalcomPartition pp;
			palcom_auth_t authData;
			pp.fetchPartitionByName("app1");
			pp.readAuthData((const esp_partition_t *)pp.partition, &authData);
			authData.fail_count++;
			pp.writeAuthData((const esp_partition_t *)pp.partition, authData);
			pp.freePartitions();
		}

		bool compResults(void){
			if(!codeReady() || !transferReady())
				return false;
			for(int i=0; i<NUMPAD_ENTRY_SIZE; i++){
				if(entryBuffer[i] != numPadEntry[i])
					return false;
			}
			return true;
		}

		void clearResult(void){
			for(int i=0; i<NUMPAD_ENTRY_SIZE; i++)
				entryBuffer[i] = 0x00;
			entryBufferCount = 0;
		}

		void clearEntry(void){
			for(int i=0; i<6; i++)
				numPadEntry[i] = 0x00;
			numPadEntryCount = 0;
		}

		void clear(void){
			this->clearResult();
                        this->clearEntry();
			numpadTitleLabel.setText("Enter Pin");
		}

		void transferResult(void){
			if(this->codeReady()){
                                for(int i=0; i<NUMPAD_ENTRY_SIZE; i++)
                                        entryBuffer[i] = numPadEntry[i];
                                entryBufferCount = NUMPAD_ENTRY_SIZE;
                                this->clearEntry();
                        }
		}

		bool transferReady(void){
			return entryBufferCount >= NUMPAD_ENTRY_SIZE;
		}

		bool codeReady(void){
			return numPadEntryCount >= NUMPAD_ENTRY_SIZE;
		}

		void setTitleText(const char *text){
			numpadTitleLabel.setText(text);
			this->execute();
		}

		int getRemainingTime(void){
			int ret = millis() - timer;
			if(ret < lockoutTime){
				return lockoutTime - ret;
			}

			return 0;
		}

		bool unlocked(void){
			if((millis()-timer) > lockoutTime || (timer == 0 && lockoutTime == 0)){
				lockoutTime = 0;
				timer=0;
				return true;
			}
			return false;
		}

		void setLockout(int minutes){
			if(minutes <= 0)
				return;

			lockoutTime = 1000*60*minutes;
			timer = millis();
		}

		void create(lv_obj_t *parent, String title){
			this->generate(parent, pal_base);
			this->setSize(this->getHorizontalResolution(), this->getVirticalResolution());
			this->execute();
			
			numpadTitleLabel.create(parent);
			this->execute();
			numpadTitleLabel.setLongMode(LV_LABEL_LONG_SCROLL);
			numpadTitleLabel.setWidth(250);
			numpadTitleLabel.setAlignment(LV_ALIGN_TOP_MID, 100, 25);
			numpadTitleLabel.setText(title.c_str());
			this->execute();
			
			int xPadding = 5;
			int yPadding = 60;
			this->createNumButton(parent, "1", 0+xPadding, 5+yPadding);
			this->createNumButton(parent, "2", 80+xPadding, 5+yPadding);
			this->createNumButton(parent, "3", 160+xPadding, 5+yPadding);
			this->createNumButton(parent, "4", 240+xPadding, 5+yPadding);
			this->execute();

			this->createNumButton(parent, "5", 0+xPadding, 65+yPadding);
			this->createNumButton(parent, "6", 80+xPadding, 65+yPadding);
			this->createNumButton(parent, "7", 160+xPadding, 65+yPadding);
			this->createNumButton(parent, "8", 240+xPadding, 65+yPadding);
			this->execute();

			this->createNumButton(parent, "9", 0+xPadding, 125+yPadding);
			this->createNumButton(parent, "0", 80+xPadding, 125+yPadding);
			this->execute();

			this->createDeleteButton(parent, 160+xPadding, 125+yPadding);
			this->execute();
		}
};
