int Messaging_pageContext = 0;
lv_obj_t *Messageing_generalRecvText = NULL;
lv_obj_t *Messageing_generalSendText = NULL;

int selectedHash = -1;
int selectedFriend = -1;
char friendHash[33];
char selectedFriendName[128];

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

bool pks_fetchHashes=true;

class PalcomKeySharing: public PalcomScreen{
	private:
  		lv_obj_t *tabDescriptor = NULL;
  		lv_obj_t *tabOne = NULL;
  		lv_obj_t *tabTwo = NULL;
  		lv_obj_t *tabThree = NULL;
  		lv_obj_t *tabFour = NULL;
  		size_t lastPublicSize = 0;
  		size_t lastEncryptedSize = 0;
  		float msgCheckTimer = millis();

  		string friendName = "";
  		string friendFile = "";

  		uint8_t hashBuffer[25];
		string hashText[25];
		int buttonCount = 0;

  		size_t getPublicMsgSize(){
    			msgCheckTimer = millis();
    			if (!SD.exists(pfs_public_msg_log))
      				return 0;

    			File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
    			size_t msgFileSize = msgFile.size();
    			msgFile.close();
    			return msgFileSize;
  		}
  
		static void Messaging_handleReceivedKey(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			uint8_t* val = (uint8_t *)lv_event_get_user_data(e);
    			selectedHash = (int)*val;
    			newPacketReceived = true;
  		}

  		static void Messaging_handleShareKey(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			palcomRadio.sendFriendKey();
  		}

  		static void Messaging_handleHomepage(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			Messaging_pageContext = 1;
  		}

  		static void Messaging_handleApproveKey(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;

    			PalcomFS pfs;
    			if(pfs.getFilenameByPos(selectedHash, pfs_dir_requests) == NULL){
      				return;
    			}

    			sprintf((char *)selectedFriendName, "%s", pfs.getFilenameByPos(selectedHash, pfs_dir_requests));

			pfs.clearFileBuffer();

    			// Read key data     
    			sprintf((char*)fileNameBuffer, "%s/%s", pfs_dir_requests, selectedFriendName);
    			File node = SD.open(fileNameBuffer);      
    			size_t keySize = node.size();
    			node.read(fileData, keySize);
    			node.close();
    			SD.remove(fileNameBuffer);

    			// Make sure root friends dir exists
    			if(!SD.exists(pfs_dir_friends)){
      				SD.mkdir(pfs_dir_friends);
    			}

    			// Make the directory for your new friend.
    			sprintf(fileNameBuffer, "%s/%s", pfs_dir_friends, selectedFriendName);
    			if(!SD.exists(fileNameBuffer)){
      				SD.mkdir(fileNameBuffer);
    			}
    
			// Write Hash
    			sprintf(fileNameBuffer, "%s/%s/hash", pfs_dir_friends, selectedFriendName);
    			Serial.printf("Writing hash to file : %s\n", fileNameBuffer);
    			node = SD.open(fileNameBuffer, FILE_WRITE);
    			node.write((uint8_t *)selectedFriendName, 33);
    			node.close();
	
    			//Write key
    			sprintf((char *)fileNameBuffer, "%s/%s/key", pfs_dir_friends, selectedFriendName);
    			Serial.printf("Writing key to file : %s\n", fileNameBuffer);
    			node = SD.open(fileNameBuffer, FILE_WRITE);
    			node.write(fileData, keySize);
    			node.close();

    			// Write Name File
    			sprintf(fileNameBuffer, "%s/%s/name", pfs_dir_friends, selectedFriendName);
    			node = SD.open(fileNameBuffer, FILE_WRITE);
    			node.printf("My Fren");
    			node.close();

    			selectedHash = -1;
    			newPacketReceived = true;
			pks_fetchHashes = true;
  		}

		void populateHashBuffer(void){
			if(SD.exists(pfs_dir_requests)){
                                File root = SD.open(pfs_dir_requests);
                                int buttonSpacing = 60;
                                buttonCount = 0;
                                while(true){
                                        if(buttonCount > 25){
                                                break;
                                        }
                                        File node = root.openNextFile();
                                        if(!node){
                                                break;
                                        }
                                        hashBuffer[buttonCount] = buttonCount;
					hashText[buttonCount] = node.name();
                                        node.close();
                                        buttonCount++;
                                }
                                root.close();
                        }
		}

  		static void Messaging_handleDenyKey(lv_event_t *e){
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      				return;
    			
			if(SD.exists(pfs_dir_requests)){
      				int buttonCount = 0;
      				File root = SD.open(pfs_dir_requests);
      				while(true){
        				if(buttonCount > 25)
          					break;
        				File node = root.openNextFile();
        				if(!node)
          					break;
        				if(buttonCount == selectedHash){
          					sprintf((char *)fileData, "%s/%s", pfs_dir_requests, node.name());
          					SD.remove((const char *)fileData);
          					node.close();
          					break;
        				}
        				node.close(); 
      				}
      				root.close();
    			}
    			selectedHash = -1;
    			newPacketReceived = true;
			pks_fetchHashes = true;
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
			this->execute();

			PalcomLabel pLabel;
                        PalcomButton pButton;
                        PalcomTextarea pTextarea;

			// Back button
			PalcomImageButton backButton;;
                        backButton.create(screen);
                        defaultImagebuttonStyle.init();
                        backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        backButton.setButtonImage(NULL, &BackIcon, NULL);
                        backButton.setSizeRaw(50, 50);
                        backButton.setSimpleCallback(Messaging_handleHomepage);
                        backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
	                this->execute();

			pButton.create(screen);
			this->execute();
			defaultButtonStyle.init();
                        pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                        pButton.setSize(30, 30);
                        pLabel.create(pButton.getObj());
                        pLabel.setText("Share Key");
                        pLabel.center();
                        pButton.setLabel(pLabel);
                        pButton.setSimpleCallback(Messaging_handleShareKey);
                        pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 100, 0);
                        this->execute();

			for(int i=0; i<buttonCount; i++){
                                int buttonSpacing = 60;
                                        if(buttonCount >= 25){
                                                break;
					}
                                        pButton.create(screen);
					defaultButtonStyle.init();
                        		pButton.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
                                        pButton.setSize(100, 30);
                                        pLabel.create(pButton.getObj());
                                        pLabel.setText(hashText[i].c_str());
                                        pLabel.center();
                                        pButton.setLabel(pLabel);
                                        hashBuffer[buttonCount] = buttonCount;
                                        pButton.setValuedCallback(Messaging_handleReceivedKey, &hashBuffer[buttonCount]);
                                        pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, -5, 60+(buttonSpacing*buttonCount));    
                                        this->execute();
                        }
  		}

	  	void approvalView(){
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

	    		tabMenu.create(screen);
    
	    		tabMenu.addTab(0, "Key Share");
    
	    		pButton.create(tabMenu.getTab(0));
	    		pButton.setSize(30, 30);
	    		pLabel.create(pButton.getObj());
	    		pLabel.setText("Approve");
	    		pLabel.center();
	    		pButton.setLabel(pLabel);
    			pButton.setSimpleCallback(Messaging_handleApproveKey);
    			pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 25, 0);

	    		pButton.create(tabMenu.getTab(0)); 
	    		pButton.setSize(30, 30);
	    		pLabel.create(pButton.getObj());
	    		pLabel.setText("Deny");
	    		pLabel.center();
	    		pButton.setLabel(pLabel);
	    		pButton.setSimpleCallback(Messaging_handleDenyKey);
	    		pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 150, 0);
	    		lv_task_handler();
	    		return;
	  	}

	public:
 		PalcomKeySharing(){
    			selectedHash = -1;
  		}
  		void resetPage(void) {
    			this->setBuildRequired(true);
    			Messaging_pageContext = 0;
    			selectedFriend = -1;
    			selectedHash = -1;
			pks_fetchHashes = true;
    			this->globalDestroy();
    			this->destroy();
  		}

  		void generateObjects(void){
    			if(selectedHash != -1){
      				this->approvalView();
      				lv_task_handler();
      				return;
    			}
    	
    			this->mainView();
    			lv_task_handler();
  		}

  		int run(void) {
    			if (this->getBuildRequired()){
				if(pks_fetchHashes){
					populateHashBuffer();
					pks_fetchHashes = false;
				}
      				this->load();
      				this->setBuildRequired(false);
    			}
    			
    	
			this->execute();
	
    			if (Messaging_pageContext <= 5) {
      				if (newPacketReceived) {
        	   			this->globalDestroy();
        	   			this->destroy();
        	   			lv_task_handler();
        	   			this->setBuildRequired(true);
        				newPacketReceived = false;
      				}
    			}

    			if(Messaging_pageContext == 1){
      				resetPage();
      				return 1;
    			}
    			return 5;
  		}
}palcomKeySharing;
