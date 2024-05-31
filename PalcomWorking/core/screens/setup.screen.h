int systemSetup_contextControl = 0;
class PalcomSetup : public PalcomScreen{
  	private:
		//ESP32Initalizer initer;
		PalcomPinpad pinpad;
		PalcomSetupForm setupForm;
    		bool initalized = false;
		bool allocated = false;
		bool loginFileExists = false;

    	
		bool checkKb(){
      			Wire.requestFrom(0x55, 1);
      			return Wire.read() != -1;
    		}
    
		static bool getTouch(int32_t x, int16_t y){
        		uint8_t rotation = tft.getRotation();
        		if(!touch->read() || x == NULL || y == NULL){
            			return false;
        		}
        		TP_Point t = touch->getPoint(0);
        		switch(rotation){
        			case 1:
            				x = t.y;
            				y = tft.height() - t.x;
            				break;
        			case 2:
            				x = tft.width() - t.x;
            				y = tft.height() - t.y;
            				break;
        			case 3:
            				x = tft.width() - t.y;
            				y = t.x;
            				break;
        			case 0:
        			default:
            				x = t.x;
            				y = t.y;
        		}
        		Sleep_interactionCtx = 0;
        		Sleep_timer = millis();
        		Serial.printf("R:%d X:%d Y:%d\n", rotation, x, y);
        		return true;
    		}
    
		const lv_img_dsc_t *img_src[1] = {&palcomLogo};
    		void displaySplash(void){
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScreenScrollDirection(LV_DIR_VER);
      	
      			this->setBgImage(img_src);
			this->execute();
    		}


	    	void keygenView(void){
	        	lv_obj_t *screen = this->getScreen();
	        	if(screen == NULL){
		          	this->globalDestroy();
		          	this->create();
		          	screen = this->getScreen();
	        	}
	        	this->setFullScreen();
	        	this->setScreenScrollDirection(LV_DIR_VER);
			string msg = "Generating Key Pair\n";

			int y = 8;
			int x = 10;
	        	PalcomLabel pLabel;
	        	pLabel.create(screen);
	        	pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
	        	pLabel.setWidth(320);
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
	        	pLabel.setText(msg.c_str());
			this->execute();

			PalcomCrypto pcry;
	        	//generateKeyPair(true);
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Initalizing RSA Variables...";
                        pLabel.setText(msg.c_str());
                        this->execute();
			pcry.rsaKeyGen.initalizeVars();
			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating RNG Seed...";
                        pLabel.setText(msg.c_str());
                        this->execute();
                        pcry.rsaKeyGen.generateRngSeed();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating Key Pair...";
                        pLabel.setText(msg.c_str());
                        this->execute();
                        pcry.rsaKeyGen.generateKeyPair();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Exporting Key Pair...";
                        pLabel.setText(msg.c_str());
                        this->execute();
                        pcry.rsaKeyGen.exportKeys();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                       	this->execute();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Writing Public Key To SD Card...";
                        pLabel.setText(msg.c_str());
                        this->execute();
                        pcry.rsaKeyGen.storeExportedPublicKey();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();

                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Writing Private Key To SD Card...";
                        pLabel.setText(msg.c_str());
                       	this->execute();
                        pcry.rsaKeyGen.storeExportedPrivateKey();
                        pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();

			pcry.rsaKeyGen.freeVars();

			pLabel.setAlignment(LV_ALIGN_TOP_MID, x, y);
			msg += "Generating public key hash file...";
                        pLabel.setText(msg.c_str());
                        this->execute();
			pcry.generatePublicHash(true);
			msg += "SUCCESS\n";
                        pLabel.setText(msg.c_str());
                        this->execute();
       	 		Setup_setupControl = 1;
    		}


	public:
	    	bool buildRequired = true;
		bool showPinpad = true;
		String errorMsg = "";

	    	void generateObjects(void){
			/*
			 * Initalize all the stuff required for the system to work.
			 * */
	      		if(!initalized){
				initer.pinInit();
				initer.aceButtonInit();
				initer.semaphoreInit();
				initer.lcdInit();
				initer.touchscreenInit();
				kbDected = checkKb();
				initer.lvglInit();
				displaySplash();
				initer.setupSD();
                        	initer.setupRadio();

			/*	try{
					PalcomCrypto pcry;
                        		pcry.generatePublicHash(true);
				}catch(CoreException e){
					String msg = "PalcomSetup::generateObject() - Failed to initalize system.\n\t";
					msg += e.what();
					if(e.errorCode() > 0){ // Critical error caught
						throw CoreException(msg.c_str(), 0x01);
					}else{
						e.log("PalcomSetup::generateObject()");
					}
				}*/

        			initalized = true;
				this->setBuildRequired(true);
				
				Serial.printf("System Initalized.\n");
				return;
      			}


			/*
			 * Check if we need to setup the system
			 * .*/
      			if(SD.exists(pfs_config)){
				loginFileExists = true;
        			return;
			}
			this->execute();

			/*
			 * Configure the screen for this context.
			 * */
      			lv_obj_t *screen = this->getScreen();
      			if(screen == NULL){
        			this->globalDestroy();
        			this->create();
        			screen = this->getScreen();
      			}
      			this->setFullScreen();
      			this->setScrollMode(LV_SCROLLBAR_MODE_OFF);
			this->execute();

			if(pinpad.codeReady() && pinpad.transferReady()){
				if(pinpad.compResults()){
					showPinpad = false;
				}else{
					errorMsg = "Pins don't match\n";
					pinpad.clearResult();
					pinpad.clearEntry();
				}
			}
			
			if(showPinpad){
				if(pinpad.codeReady() && !pinpad.transferReady()){
					pinpad.transferResult();
					this->destroy();
					pinpad.create(screen, "Confirm Passcode");
				}else{
					this->destroy();
					if(errorMsg == "")
						pinpad.create(screen, "Create Passcode");
					else
						pinpad.create(screen, errorMsg.c_str());
				}
			}else{
				this->destroy();
				setupForm.create(screen, "Continue Setup");
			}
			this->execute();
    		}

    		void resetPage(){
			this->setBuildRequired(true);
        		systemSetup_contextControl = 0;
        		this->globalDestroy();
        		this->destroy();
			Setup_setupControl = 0;
			loginFileExists = false;
    		}

    		int run(){
			/*
			 * Ensure page is setup
			 * */
      			if(this->getBuildRequired()){
        			this->setBuildRequired(false);
        			systemSetup_contextControl = 0;
        			this->load();
      			}
      			this->execute();

      			if(loginFileExists) {
				Serial.printf("Loading Login Screen\n");
        			resetPage();
        			return 0; // change screen to login page.
      			}
      			this->execute();

			if(pinpad.codeReady() && showPinpad){
				this->setBuildRequired(true);
				this->destroy();
			}

			if(setupForm.formSubmitted()){
				PalcomFS pfs; 
				PalcomPartition pp;
				pp.getAllPartitions();
				size_t writeSize = 6;
				
				PalcomHash phash;
				phash.useSHA256();
				phash.run((unsigned char *)pinpad.entryBuffer, pinpad.entryBufferCount);
				palcom_auth_t authData;

				// Fetch Pin Hash
				String grab = phash.getResultStr();
				if(phash.getResultSize() > 32)
					throw CoreException("PalcomSetup::run() - Invalid hash size.", 0x01);
				strncpy((char *)authData.pin_hash, grab.c_str(), phash.getResultSize());

				// Fetch Pmode
				authData.paranoia_mode = setupForm.paranoiaMode;

				// Generate AES Key and IV
				PalcomAes aes;
				if(!aes.generate()){
					throw CoreException("PalcomSetup::run() - Failed to generate AES keys.\n", 0x02);
				}
				Serial.printf("AES Key Generated!\n");
				for(int i=0; i<32; i++)
					authData.aes_key[i] = aes.key[i];
				for(int i=0; i<16; i++)
					authData.aes_iv[i] = aes.iv[i];
				aes.clearKey();
				aes.clearIv();

				// Store sensitive data in flash memory.
				while(pp.fetchPartition()){
					String comp = (const char *)pp.partition->label;
					if(comp != "app1")
						continue;

					pp.writeAuthData((const esp_partition_t *)pp.partition, authData);
					break;
				}
				pp.freePartitions();
				
				// Store non-sensitive config data (user name, lock timer, etc)
				palcom_config_t configData;
				configData.lock_timer = setupForm.lockTimer;
				strncpy((char *)configData.user_name, setupForm.name.c_str(), 20);
				configData.screen_brightness = 100;
				pfs.storeConfigData(configData);
				
				this->globalDestroy();
                                this->destroy();
                                this->execute();
                                keygenView();
                                PalcomCrypto pcry;
                                pcry.generatePublicHash(true);
                                resetPage();
                                return 0; // change screen to login page.
			}

			this->execute();

			/*
			 * If login file exists, exit setup page.
			 * */
			

      			return -1; // loop the setup page.
    		}
}palcomSetup;
