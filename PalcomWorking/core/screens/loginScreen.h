// Call back variables
int login_context = 0;

class PalcomLoginScreen : public PalcomScreen{
  	private:
		PalcomPinpad pinpad;

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
		String errorMsg = "";
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
			this->execute();

			if(errorMsg == "")
				pinpad.create(screen, "Enter Passcode");
			else
				pinpad.create(screen, errorMsg.c_str());

			this->execute();
    		}

    		void resetPage(){
      			this->globalDestroy();
      			this->destroy();
        		this->setBuildRequired(true);
			this->clearScreenError();
			errorMsg = "";
			login_context = 0;
    		}

    		bool run(void){
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			this->load();
      				login_context = 0;
      			}

			if(pinpad.codeReady()){
				pinpad.transferResult();
				PalcomHash phash;
                                phash.useSHA256();
                                phash.run((unsigned char *)pinpad.entryBuffer, pinpad.entryBufferCount);

                                // Fetch Pin Hash
                                String grab = phash.getResultStr();
                                if(phash.getResultSize() > 32)
                                        throw CoreException("PalcomLogin::run() - Invalid hash size.", 0x01);
				
				PalcomPartition pp;
				if(!pp.fetchPartitionByName("app1")){
					throw CoreException("PalcomLogin::run() - Failed to find flash partition 'app1'", 0x02);
				}

                                palcom_auth_t authData;
				pp.readAuthData(pp.partition, &authData);
				pp.freePartitions();

				PalcomFS pfs;
				pfs.clearAllBuffers();
				pfs.addToFiledata((char *)authData.pin_hash, 32); // 32 is the size of the hash we used.
				pfs.addToCompbuffer((char *)grab.c_str(), 32);

				if(pfs.equalBuffers()){
					return true;
				}else{
					pinpad.clear();
					errorMsg = "Invalid Pin.";
					pinpad.setTitleText(errorMsg.c_str());
				}

			}
      			this->execute();

      			return false;
    		}
};
