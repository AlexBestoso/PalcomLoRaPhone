// Call back variables
int login_context = 0;

class PalcomLoginScreen : public PalcomScreen{
  	private:
    		static void Login_handleSubmit(lv_event_t *e){      
      			if (lv_event_get_code(e) != LV_EVENT_RELEASED)
        			return;
      			PalcomTextarea login_user;
      			PalcomTextarea login_pass;
      			login_user.loadGlobal(1);
      			login_pass.loadGlobal(2);
      			string username = login_user.getText();
      			string password = login_pass.getText();
      
      			if((username == "" || password == "")){
        			login_context = 0;
				PalcomScreenError = 1;
        			return;
      			}
      			char pass_hash[33];
			PalcomFS pfs;
			PalcomCrypto pcry;
			pcry.hash.useSHA256();
			pcry.hash.run((unsigned char *)password.c_str(), password.length());
      			string hash = pcry.hash.getResult();

      			File hashFile = SD.open("/login.hash", FILE_READ);
      			if(!hashFile){
        			Serial.println("Failed to open file.");
        			while(1){delay(5);}
      			}
      			size_t hashFileSize = hashFile.size();
			pfs.clearFileBuffer();
      			hashFile.read(fileData, hashFileSize);
      			hashFile.close();
	

      			int iter = username.length();
			pfs.clearCompBuffer();
      			for(int i=0; i<username.length(); i++){
        			compBuffer[i] = username[i];
      			}
      			compBuffer[iter] = '\r';
      			compBuffer[iter+1] = '\n';
      			iter+= 2;

      			for(int i=0; i<pcry.hash.getResultSize(); i++){
        			compBuffer[iter] = hash[i];
        			iter++;
      			}
			compBuffer[iter] = 0x00;

      			if(strcmp(compBuffer, (const char *)fileData)){
          			login_context = 2;
				PalcomScreenError = 1;
          			return;
      			}
      			login_context = 1;
    		}
  	
	public:
    		void generateObjects(void){
      			// Configure Screen
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
			this->setScrollMode(LV_SCROLLBAR_MODE_OFF);

      			// Title Message
      			PalcomLabel pLabel;
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
			int err = this->getScreenError();
			if(err == 1){
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 115, 10);
        			pLabel.setText("Invalid Credentials");
			}else if(login_context == 3){
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 125, 10);
        			pLabel.setText("Logged Out");
			}else{
      				pLabel.setAlignment(LV_ALIGN_TOP_MID, 125, 10);
        			pLabel.setText("Enter Login");
			}
			this->execute();

      			// Username Input
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
      			pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 35+5);
      			pLabel.setText("Username: ");
			PalcomTextarea username;
                        username.createGlobal(screen, 1);
                        defaultTextareaStyle.initStyle();
                        username.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
                        username.setCursorClickPos(false);
                        username.setTextSelection(false);
                        username.setSize(175, 23);
                        username.setText("");
                        username.setMaxLength(18);
                        username.setOneLine(true);
                        username.setAlignment(LV_ALIGN_TOP_MID, 20, 30+5);
			this->execute();

      			// Password Input
      			pLabel.create(screen);
      			pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      			pLabel.setWidth(320);
      			pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 80+10);
      			pLabel.setText("Password: ");
      			PalcomTextarea password;
      			password.createGlobal(screen, 2);
			defaultTextareaStyle.initStyle();
			password.setStyle(defaultTextareaStyle.getStyle(), defaultTextareaStyle.getFocusedStyle());
      			password.setCursorClickPos(false);
      			password.setTextSelection(false);
      			password.setSize(175, 23);
      			password.setText("");
      			password.setMaxLength(18);
      			password.setOneLine(true);
      			password.setPasswordMode(true);
      			password.setAlignment(LV_ALIGN_TOP_MID, 20, 70+10);
			this->execute();

      			// Submit Button
      			PalcomButton submit;
      			submit.create(screen);
			defaultButtonStyle.initStyle();
			submit.setStyle(defaultButtonStyle.getStyle(), defaultButtonStyle.getPressedStyle());
      			submit.setSize(110, 40);
      			pLabel.create(submit.getObj());
      			pLabel.setText("Login");
      			pLabel.center();
      			submit.setLabel(pLabel);
      			submit.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, -5, 65+25);
      			submit.setSimpleCallback(Login_handleSubmit);
			this->execute();
    		}

    		void resetPage(){
      			this->globalDestroy();
      			this->destroy();
        		this->setBuildRequired(true);
			this->clearScreenError();
			login_context = 0;
    		}

    		bool run(void){
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			this->load();
      				login_context = 0;
      			}

      			this->execute();

			if(this->isScreenError()){
        			this->globalDestroy();
        			this->destroy();
        			this->setBuildRequired(true);
        			return false;	
			}
      			if(login_context == 2){
        			this->globalDestroy();
        			this->destroy();
        			this->setBuildRequired(true);
        			return false;
      			}
      			if(login_context == 1){
        			this->globalDestroy();
        			this->destroy();
        			this->setBuildRequired(true);
        			login_context = 3;
        			return true;
      			}
      			return false;
    		}
};
