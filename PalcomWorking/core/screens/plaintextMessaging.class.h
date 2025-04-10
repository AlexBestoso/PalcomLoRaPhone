int ptm_pageContext = 0;

class PalcomPlaintextMessaging: public PalcomScreen{
	private:
  		size_t lastPublicSize = 0;
  		float msgCheckTimer = millis();
		bool firstRun = true;
		string myInput = "";

  		size_t getPublicMsgSize(){
    			msgCheckTimer = millis();
    			if (!SD.exists(pfs_public_msg_log))
      				return 0;

    			File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
    			size_t msgFileSize = msgFile.size();
    			msgFile.close();
    			return msgFileSize;
  		}
  
		static void ptm_handlePlaintextSend(lv_event_t *e) {
    			if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        			return;

    			PalcomTextarea pTextarea;
    			pTextarea.loadGlobal(2);
    			PalcomFS pfs;
    			string msg = pfs.getCallsign();
			msg += ":\n";
			msg += pTextarea.getText();
		
    			if (strlen(pTextarea.getText()) <= 0) {
      				Serial.printf("No message, not sending.\n");
      				return;
    			}
			palcomRadio.sendQueueAdd((char *)msg.c_str(), msg.size(), palcomRadio.publicCode_int);
			palcomRadio.appendGeneralMessage(msg, false);
    			pTextarea.setText("");
  		}

		static void Messaging_handleHomepage(lv_event_t *e){
                        if (lv_event_get_code(e) != LV_EVENT_CLICKED)
                                return;
                        ptm_pageContext = 1;
                }

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

	
			PalcomMessage pMessage;
			pMessage.backgroundH = 140;
			pMessage.backgroundW = pMessage.getHorizontalResolution();
			pMessage.backgroundY = 50;
			pMessage.messageW = 150;
			pMessage.messageY = 0;
			pMessage.messageX = 0;
			pMessage.createGlobal(screen, 1);
			pMessage.loadGeneralMessages();
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
                        messageSendInput.setSubmitHandlerr(ptm_handlePlaintextSend);

                        messageSendInput.create(screen);

                        defaultTextInputStyle.initStyle();
                        messageSendInput.setStyle(defaultTextInputStyle.getStyle());
                        this->execute();

			PalcomImageButton backButton;
                        backButton.create(screen);
                        defaultImagebuttonStyle.init();
                        backButton.setStyle(defaultImagebuttonStyle.getStyle(), defaultImagebuttonStyle.getPressedStyle());
                        backButton.setButtonImage(NULL, &BackIcon, NULL);
                        backButton.setSizeRaw(50, 50);
                        backButton.setSimpleCallback(Messaging_handleHomepage);
                        backButton.setRelativeAlignment(LV_ALIGN_OUT_TOP_RIGHT, 0,  50);
	    		this->execute();
  		}

	public:
 		PalcomPlaintextMessaging(){
  		}
  		void resetPage(void) {
    			this->setBuildRequired(true);
    			ptm_pageContext = 0;
    			this->globalDestroy();
    			this->destroy();
			firstRun = true;
			myInput = "";
  		}

  		void generateObjects(void){
    			this->mainView();
    			lv_task_handler();
  		}

  		int run(void) {
    			if (this->getBuildRequired()){
				if(this->firstRun){
					lastPublicSize = getPublicMsgSize();
					firstRun = false;
				}
      				this->load();
      				this->setBuildRequired(false);
    			}
    	
			lv_task_handler();
	
    			if (ptm_pageContext <= 0) {
      				if ((millis() - msgCheckTimer) > ((1000 * 60)) || newPacketReceived) {
					size_t ledgerSize = getPublicMsgSize();
        	  			if (ledgerSize != lastPublicSize || newPacketReceived) {
						// Check for text input to ensure user doesn't lose the
						// message they're typing
						PalcomTextarea grabber;
                        			grabber.loadGlobal(2);
						if(grabber.getObject() != NULL){
							myInput	= grabber.getText();
						}else{
							myInput = "";
						}

        	   				this->globalDestroy();
        	   				this->destroy();
        	   				lv_task_handler();
        	   				this->setBuildRequired(true);
						lastPublicSize = ledgerSize;
        	  			}
        				newPacketReceived = false;
        				msgCheckTimer = millis();
      				}
    			}

    			if(ptm_pageContext == 1){
      				resetPage();
      				return 1;
    			}
    			return 3;
  		}
} palcomPlaintextMessaging;
