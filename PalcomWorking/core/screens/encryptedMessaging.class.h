int Messaging_pageContext = 0;
lv_obj_t *Messageing_generalRecvText = NULL;
lv_obj_t *Messageing_generalSendText = NULL;

int selectedFriend = -1;
int activeTab = 0;
char friendHash[33];
bool pem_fetchFriends=true;
int pem_friendViewContext = 0;
int pem_buttonCount = 0;

void getFriendHash(void){
    	PalcomFS pfs;
	pfs.clearCompBuffer();
    	const char *friendHashFileName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
    	sprintf((char *)fileData, "%s/%s/hash", pfs_dir_friends, friendHashFileName);
    	File friendHashFile = SD.open((const char *)fileData, FILE_READ);
    	size_t keySize = friendHashFile.size();
    	//for(int i=0; i<33; i++)
      	//	compBuffer[i] = 0;
    	friendHashFile.read((uint8_t *)compBuffer, keySize);
    	friendHashFile.close();
    	for(int i=0; i<33; i++)
      		friendHash[i] = compBuffer[i];
}

class PalcomEncryptedMessaging: public PalcomScreen{
	private:
  		size_t lastEncryptedSize = 0;
  		float msgCheckTimer = millis();

  		string friendName = "";
  		string friendFile = "";
		string myInput = "";

  		uint8_t firendBuffer[256];
		string friendNameBuffer[256];
		string friendHashBuffer[256];

		bool firstRun = true;

  		size_t getEncryptedMsgSize(void){
			if(selectedFriend == -1){
				return 0;
			}
			Serial.printf("Nigger\n");
    			PalcomFS pfs;
    			const char *friendDirName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
			sprintf((char *)compBuffer, "%s/%s/msgLog", pfs_dir_friends, friendHashBuffer[selectedFriend].c_str());
    			msgCheckTimer = millis();
    
			Serial.printf("Fagggot\n");
    			if (!SD.exists(compBuffer))
      				return 0;
    
			Serial.printf("Cunt\n");
    			File msgFile = SD.open(compBuffer, FILE_READ);
    			size_t msgFileSize = msgFile.size();
    			msgFile.close();
			Serial.printf("Detected friend size of %ld\n", msgFileSize);
    			return msgFileSize;
  		}

  		void getFriendFile(void){
     			if(SD.exists(pfs_dir_friends)){
      				int friendCount = 0;
      				File root = SD.open(pfs_dir_friends);
      				while(true){
        				if(friendCount > 256)
          					break;
        				
					File node = root.openNextFile();
        				if(!node)
          					break;
        				
					if(friendCount == selectedFriend){
          					sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          					friendFile = (const char *)fileData;
          					node.close();
          					break;
        				}
        				node.close(); 
      				}
      				root.close();
    			}
  		}

  		void getEncryptedMsgData(void) {
    			string ret = "";
    			PalcomFS pfs;
    			const char *friendDirName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
    			sprintf((char *)compBuffer, "%s/%s/msgLog", pfs_dir_friends, friendDirName);
    			if (SD.exists(compBuffer)) {
      				File msgFile = SD.open(compBuffer, FILE_READ);
      				size_t msgFileSize = msgFile.size();
      				for (int i = 0; i < 100000; i++)
        				fileData[i] = 0;
      				lastEncryptedSize = msgFileSize;
      				msgFile.read(fileData, msgFileSize);
      				msgFile.close();
    			} else {
      				lastEncryptedSize = 0;
    			}
  		}

  		void getFriendMsgData(void){
    			if(selectedFriend <= -1){
      				return;
    			}

    			if (SD.exists(pfs_public_msg_log)) {
      				File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
      				size_t msgFileSize = msgFile.size();
      				for (int i = 0; i < 100000; i++)
        				fileData[i] = 0;
      				lastEncryptedSize = msgFileSize;
      				msgFile.read(fileData, msgFileSize);
      				msgFile.close();
    			} else {
      				lastEncryptedSize = 0;
    			}
  		}

  		static void Messaging_handleEncryptedSend(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED || selectedFriend == -1)
        			return;
    				
			Serial.printf("Sending it.\n");
    			PalcomFS pfs;
    			const char *friendDirectory = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
    
    			PalcomTextarea pTextarea;
    			pTextarea.loadGlobal(2);
    			string msg = pTextarea.getText();
    			if (msg.length() <= 0) {
      				Serial.printf("No message, not sending.\n");
      				return;
    			}
			sprintf(compBuffer, "/friends/%s/msgLog", friendDirectory);
			palcomRadio.appendFriendMessage((const char *)compBuffer, msg, false);

    			if(SD.exists(pfs_file_cryptSend)){
      				SD.remove(pfs_file_cryptSend);
    			}

    			//getFriendHash();// This resets filedata
    			if(!pfs.getPublicHash(friendHash))
      				return;
    
    			Serial.printf("Sending the hash : %s\n", friendHash);
    			sprintf((char *)compBuffer, "%s/%s/key", pfs_dir_friends, (const char*)pfs.getFilenameByPos(selectedFriend, pfs_dir_friends));
    			Serial.printf("Reading key file : %s\n", (const char *)compBuffer);
    			if(!rsaEncrypt(compBuffer, (const unsigned char *)msg.c_str(), msg.length(), pfs_file_cryptSend)){
      				Serial.printf("Encryption Failed.\n");
    			}
    			File encryptedData = SD.open(pfs_file_cryptSend, FILE_READ);
    			size_t ctextSize = encryptedData.size();
    			encryptedData.read(fileData, ctextSize);
    			encryptedData.close();
    
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      				compBuffer[i] = 0;
    			for(int i=0; i<33; i++){
      				compBuffer[i] = friendHash[i];
    			}
    			for(int i=0; i<ctextSize; i++){
      				compBuffer[i+33] = fileData[i];
    			}

			palcomRadio.sendQueueAdd((char *)compBuffer, ctextSize+33, palcomRadio.encryptedCode_int);

    			pTextarea.setText("");
  		}

  		static void Messaging_handleUpdateContact(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			bool renamed = false;
    			string newName = "";
    
    			PalcomTextarea pText;
    			pText.loadGlobal(3);
    			sprintf((char *)fileData, "%s/%s", pfs_dir_friends, pText.getText());

    			PalcomFS pfs;
    			const char *friendDirName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
    			if(friendDirName){
      				sprintf((char *)compBuffer, "%s/%s/name", pfs_dir_friends, friendDirName);

      				File nameFile = SD.open((char *)compBuffer, FILE_WRITE, O_TRUNC);
      				nameFile.write((const uint8_t*)pText.getText(), strlen(pText.getText()));
      				nameFile.close();
      				newPacketReceived = true;
    			}
  		}

  		static void Messaging_handleDeleteContact(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
      			PalcomFS pfs;
    			const char *friendDirName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
    			if(friendDirName){
      				sprintf((char *)compBuffer, "%s/%s", pfs_dir_friends, friendDirName);
      				pfs.rm((const char *)compBuffer);
    			}
      			selectedFriend = -1;
      			newPacketReceived = true;
			pem_fetchFriends = true;
                        pem_buttonCount = 0;
  		}
		

  		static void Messaging_handleSelectedFriend(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			uint8_t* val = (uint8_t *)lv_event_get_user_data(e);
			if(val == NULL)
				return;
    			selectedFriend = (int)*val;
    			newPacketReceived = true;
  		}

  		static void Messaging_handleHomepage(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			Messaging_pageContext = 1;
			newPacketReceived = true;
  		}

		static void Messaging_backToFrenSelect(lv_event_t *e){
                        if (lv_event_get_code(e) != LV_EVENT_CLICKED)
                                return;
                        selectedFriend = -1;
			newPacketReceived = true;
			pem_fetchFriends = true;
			pem_buttonCount = 0;
                }

		static void Messaging_showMessages(lv_event_t *e){
                        if (lv_event_get_code(e) != LV_EVENT_CLICKED)
                                return;
                        pem_friendViewContext = 0;
			newPacketReceived = true;
                }

		static void Messaging_showContactInfo(lv_event_t *e){
                        if (lv_event_get_code(e) != LV_EVENT_CLICKED)
                                return;
                        pem_friendViewContext = 1;
			newPacketReceived = true;
                }


  		PalcomTabMenu tabMenu;

  		void mainView(){
    			lv_obj_t *screen = this->getScreen();
    			if(screen == NULL){
      				this->globalDestroy();
      				this->create();
      				screen = this->getScreen();
    			}
    			this->setFullScreen();
    			this->setScreenScrollDirection(LV_DIR_VER);

			PalcomLabel pLabel;
                        PalcomButton pButton;
                        PalcomTextarea pTextarea;

			PalcomImageButton backButton;
                        backButton.create(screen);
                        defaultImagebuttonStyle.init();
                        backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        backButton.setButtonImage(NULL, &BackIcon, NULL);
                        backButton.setSizeRaw(50, 50);
                        backButton.setSimpleCallback(Messaging_handleHomepage);
                        backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
                        this->execute();
			
                        int buttonSpacing = 60;
			for(int i=0; i<pem_buttonCount; i++){
				pButton.create(screen);
                                defaultButtonStyle.init();
                                pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                                pButton.setSize(100, 30);
                                pLabel.create(pButton.getObj());
                                pLabel.setText(friendNameBuffer[i].c_str());
                                pLabel.center();
                                pButton.setLabel(pLabel);
                                pButton.setValuedCallback(Messaging_handleSelectedFriend, &firendBuffer[i]);
                                pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, -5, 60+(buttonSpacing*i));
                                this->execute();
			}
  		}

	  	void friendView(){
	    		Serial.printf("In friend view.\n");
	    		lv_obj_t *screen = this->getScreen();
	    		if(screen == NULL){
	      			this->globalDestroy();
	      			this->create();
	      			screen = this->getScreen();
	    		}
	    		this->setFullScreen();
	    		this->setScreenScrollDirection(LV_DIR_VER);

			PalcomLabel pLabel;
    			PalcomButton pButton;
    			PalcomTextarea pTextarea;

			PalcomImageButton backButton;
                        backButton.create(screen);
                        defaultImagebuttonStyle.init();
                        backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        backButton.setButtonImage(NULL, &BackIcon, NULL);
                        backButton.setSizeRaw(50, 50);
                        backButton.setSimpleCallback(Messaging_backToFrenSelect);
                        backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
                        this->execute();

			// messaging button
			pButton.create(screen);
                        defaultButtonStyle.init();
                        pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        pButton.setSizeRaw(50, 50);
                        pLabel.create(pButton.getObj());
                        pLabel.setText("Msg");
                        pLabel.center();
                        pButton.setLabel(pLabel);
                        pButton.setSimpleCallback(Messaging_showMessages);
                        pButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 0, 45);

			//settings button
			pButton.create(screen);
                        defaultButtonStyle.init();
                        pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        pButton.setSizeRaw(50, 50);
                        pLabel.create(pButton.getObj());
                        pLabel.setText("Info");
                        pLabel.center();
                        pButton.setLabel(pLabel);
                        pButton.setSimpleCallback(Messaging_showContactInfo);
                        pButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_LEFT, 55, 45);

			if(pem_friendViewContext == 1){
				pLabel.create(screen);
        	                pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
        	                pLabel.setWidth(320);
        	                pLabel.setAlignment(LV_ALIGN_TOP_MID, 5, 100);
        	                pLabel.setText("Name:");
	
                        	pTextarea.createGlobal(screen, 3);
                        	pTextarea.setCursorClickPos(false);
                        	pTextarea.setTextSelection(false);
                        	pTextarea.setSize(220, 23);
                        	pTextarea.setMaxLength(33);
                        	pTextarea.setOneLine(true);
                        	pTextarea.setAlignment(LV_ALIGN_TOP_MID, 15, 100);
                        	lv_task_handler();

				PalcomFS pfs;
                        	const char *friendDirName = pfs.getFilenameByPos(selectedFriend, pfs_dir_friends);
                        	if(!friendDirName){
                        	        pTextarea.setText("My Frien");
                        	}else{
                        	        sprintf((char *)compBuffer, "%s/%s/name", pfs_dir_friends, friendDirName);
					pfs.clearFileBuffer();
                        	        File nameFile = SD.open((char *)compBuffer, FILE_READ);
                        	        nameFile.read(fileData, nameFile.size());
                        	        nameFile.close();
                        	        pTextarea.setText((const char *)fileData);
                        	}
                        	lv_task_handler();

                        	pButton.create(screen);
				defaultButtonStyle.initStyle();
				pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        	pButton.setSize(25, 30);
                        	pLabel.create(pButton.getObj());
                        	pLabel.setText("Update");
                        	pLabel.center();
                        	pButton.setLabel(pLabel);
                        	pButton.setSimpleCallback(Messaging_handleUpdateContact);
                        	pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 50, 195);
                        	lv_task_handler();

				pButton.create(screen);
				defaultButtonStyle.initStyle();
				pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        	pButton.setSize(25, 30);
                        	pLabel.create(pButton.getObj());
                        	pLabel.setText("Delete");
                        	pLabel.center();
                        	pButton.setLabel(pLabel);
                        	pButton.setSimpleCallback(Messaging_handleDeleteContact);
                        	pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 150, 195);
                        	lv_task_handler();
			}else{
				PalcomMessage pMessage;
				pMessage.backgroundH = 140;
				pMessage.backgroundW = pMessage.getHorizontalResolution();
				pMessage.backgroundY = 50;
				pMessage.messageW = 150;
				pMessage.messageY = 0;
				pMessage.messageX = 0;
				pMessage.createGlobal(screen, 1);
				pMessage.loadEncryptedMessages(friendHashBuffer[selectedFriend]);
				this->execute();
	
				PalcomTextInput messageSendInput;
                        	messageSendInput.setInputWidth(270+50);
                        	messageSendInput.setInputHeight(50);
                        	messageSendInput.setInputY(180+10);

                        	messageSendInput.setLabelText("");

                        	messageSendInput.setTextareaId(2);
                        	messageSendInput.setTextareaWidth(140+70);
                        	messageSendInput.setTextareaHeight(18);
                        	messageSendInput.setTextareaX(0);
                        	messageSendInput.setTextareaY(-8);
                        	messageSendInput.setMaxLength(1024);
				if(myInput != ""){
					messageSendInput.setDefaultText(myInput.c_str());
				}

                        	messageSendInput.setButtonText("Send");
                        	messageSendInput.setSubmitHandlerr(Messaging_handleEncryptedSend);

                        	messageSendInput.create(screen);

                        	defaultTextInputStyle.initStyle();
                        	messageSendInput.setStyle(defaultTextInputStyle.getStyle());
                        	this->execute();
			}
  		}
	
		void fetchFriendList(void){
			PalcomFS pfs;
			if(SD.exists(pfs_dir_friends)){
                                File root = SD.open(pfs_dir_friends);
				while(true){
                                        if(pem_buttonCount > 256)
                                                break;
                                        File node = root.openNextFile();
                                        if(!node)
                                                break;
                                        sprintf((char *)compBuffer, "%s/%s/name", pfs_dir_friends, node.name());
					friendHashBuffer[pem_buttonCount] = node.name();
                                        File nameFile = SD.open(compBuffer);
                                        if(!nameFile){
						friendNameBuffer[pem_buttonCount] = "My Friend";
                                        }else{
						pfs.clearFileBuffer();
                                                nameFile.read(fileData, nameFile.size());
                                                nameFile.close();
                                                friendNameBuffer[pem_buttonCount] = (const char *)fileData;
                                        }

                                        firendBuffer[pem_buttonCount] = pem_buttonCount;
                                        node.close();
                                        pem_buttonCount++;
                                }
			}
		}
	public:
 		PalcomEncryptedMessaging(){
  		}
  		void resetPage(void) {
    			this->setBuildRequired(true);
    			Messaging_pageContext = 0;
    			selectedFriend = -1;
			pem_fetchFriends = true;
			pem_buttonCount = 0;
			firstRun = true;
			pem_friendViewContext = 0;
    			this->globalDestroy();
    			this->destroy();
  		}

  		void generateObjects(void){
    			if(selectedFriend != -1){
      				this->friendView();
      				lv_task_handler();
      				return;
    			}
    
    			this->mainView();
    			lv_task_handler();
  		}

  		int run(void) {
    			if (this->getBuildRequired()){
				if(this->firstRun){
                                        lastEncryptedSize = getEncryptedMsgSize();
                                        firstRun = false;
                                }
				if(pem_fetchFriends){
					this->fetchFriendList();
					pem_fetchFriends = false;
				}
      				this->load();
      				this->setBuildRequired(false);
    			}
    			
			lv_task_handler();
	
    			if (Messaging_pageContext <= 0){
      				if ((millis() - msgCheckTimer) > ((1000 * 60)) || newPacketReceived) {
					size_t ledgerSize = getEncryptedMsgSize();
        	  			if (ledgerSize != lastEncryptedSize || newPacketReceived) {
						// Check for text input to ensure user doesn't lose the
                                               	// message they're typing
						
						if(pem_friendViewContext != 1 && selectedFriend != -1){
                                               		PalcomTextarea grabber;
                                               		grabber.loadGlobal(2);
                                               		if(grabber.getObject() != NULL){
                                               		        myInput = grabber.getText();
                                               		}else{
                                               		        myInput = "";
                                               		}
						}
        	   				this->globalDestroy();
        	   				this->destroy();
        	   				lv_task_handler();
        	   				this->setBuildRequired(true);
        	  			}
				}
        			newPacketReceived = false;
        			msgCheckTimer = millis();
    			}

    			if(Messaging_pageContext == 1){
      				resetPage();
      				return 1;
    			}
    			return 4;
  		}
}palcomEncryptedMessaging;
