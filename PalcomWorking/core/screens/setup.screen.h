int systemSetup_contextControl = 0;
class PalcomSetup : public PalcomScreen{
  	private:
		PalcomPinpad pinpad;
		PalcomSetupForm setupForm;
		PalcomCrypto pcry;
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
			if(!pcry.rsaKeyGen.genKeys()){
				throw CoreException("PalcomSetup::keygenView() - Failed to generate RSA key pair.", 0x01);
			}
    		}

		void encryptPrivateKey(void){
			PalcomFS pfs;
			pcry.aes.fetchMasterKey();
			pfs.clearFileBuffer();
			pfs.clearCompBuffer();

			pfs.fd = SD.open(pfs_file_keysPrivate, FILE_READ);
			if(!pfs.fd){
				pfs.close();
				throw CoreException("PalcomSetup::encryptPrivateKey() - Failed to open private key.", 0x01);
			}
			size_t keySize = pfs.fd.size();
			size_t originalSize = keySize;
			pfs.fd.read(fileData, keySize);
			pfs.close();
			// Key has been obtained as well as it's size.
			Serial.printf("Private Key Size : %ld\n", keySize);

			
			// Determine the size of the output buffer.
			size_t cipherTextSize = pcry.aes.calcOutputSize(keySize);
			Serial.printf("Predicted Cipher Text Size: %ld\n", cipherTextSize);

			String chalOne = (const char *)&fileData;

			// Attempting Encryption.
			pcry.aes.crypt(true, (unsigned char *)fileData, keySize, (unsigned char *)&compBuffer, cipherTextSize);


			// Store the now encrypted private key.
			pfs.fd = SD.open(pfs_file_keysPrivate, FILE_WRITE, O_TRUNC);
			if(!pfs.fd){
				pfs.close();
				throw CoreException("PalcomSetup::encryptPrivateKey() - Failed to truncate the private key file.", 0x02);
			}
			pfs.fd.write((unsigned char *)compBuffer, cipherTextSize);
			pfs.close();
			
			// Confirm encryption wa successful.
			pcry.aes.fetchMasterKey();
			pfs.clearFileBuffer();
			pfs.clearCompBuffer();
			pfs.fd = SD.open(pfs_file_keysPrivate, FILE_READ);
                        if(!pfs.fd){
                                pfs.close();
                                throw CoreException("PalcomSetup::encryptPrivateKey() - Failed to open private key.", 0x03);
                        }
                        keySize = pfs.fd.size();
                        pfs.fd.read(fileData, keySize);
                        pfs.close();

			// Attempt Decryption.
			cipherTextSize = pcry.aes.calcOutputSize(keySize);
			pcry.aes.crypt(false, (unsigned char *)fileData, keySize, (unsigned char *)&compBuffer, cipherTextSize);

			String chalTwo = (const char *)&compBuffer;

			if(chalOne.length() != chalTwo.length()){
				throw CoreException("PalcomSetup::encryptPrivateKey() - Challenge lengths inequal.\n", 0x4);
			}
			
			for(int i=0; i<chalOne.length(); i++){
				if(chalOne[i] != chalTwo[i]){
					Serial.printf("Failed on iteration '%d' at %x vs %x\n", i, chalOne[i], chalTwo[i]);
					throw CoreException("PalcomSetup::encryptPrivateKey() - Failed to validate successful encryption of the private key.", 0x05);
				}
			}
			
			pfs.clearCompBuffer();
			pfs.clearFileBuffer();
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
			
			if(showPinpad){
				this->pinpad.create(screen, "Create Passcode");
				this->execute();
			}else{
				this->setupForm.create(screen, "Continue Setup");
				this->execute();
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
			try{
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

				if(showPinpad && pinpad.codeReady()){
					this->execute();
					if(!pinpad.transferReady()){
						pinpad.transferResult();
						pinpad.setTitleText("Confirm Passcode");
						this->execute();
					}else if(pinpad.compResults()){
						showPinpad = false;
						this->setBuildRequired(true);
						this->destroy();
						this->execute(); // ?
					}else{
						pinpad.clear();
						pinpad.setTitleText("Passcodes didn't match");
						this->execute();
					}
				}else if(setupForm.formSubmitted()){
					this->destroy();
					lv_obj_t *screen = this->getScreen();
                        		if(screen == NULL){
                         		       	this->globalDestroy();
                         		       	this->create();
                         			screen = this->getScreen();
                        		}
                        		this->setFullScreen();
					this->setScreenScrollDirection(LV_DIR_VER);
                        		this->execute();
					String infoMsg = "Setting up your device...\n";
					PalcomLabel lab;
					int x=10, y=9;
					lab.create(screen);
					lab.setLongMode(LV_LABEL_LONG_SCROLL);
					lab.setWidth(320);
					lab.setAlignment(LV_ALIGN_OUT_TOP_MID, x, y);
					lab.setText(infoMsg.c_str());
					this->execute();


					PalcomFS pfs; 
					PalcomPartition pp;
					size_t writeSize = 6;
				
					infoMsg += "Creating passcode hash...\n";
					lab.setText(infoMsg.c_str());
					this->execute();

					PalcomHash phash;
					phash.useSHA256();
					phash.run((unsigned char *)pinpad.entryBuffer, pinpad.entryBufferCount);
					palcom_auth_t authData;

					infoMsg += "Validating passcode hash\n";
					lab.setText(infoMsg.c_str());
					this->execute();

					// Fetch Pin Hash
					String grab = phash.getResultStr();
					if(phash.getResultSize() != 32)
						throw CoreException("PalcomSetup::run() - Invalid hash size.", 0x01);
					for(int i=0; i<32; i++){
						this->execute();
						authData.pin_hash[i] = grab[i];
					}

					infoMsg += "Fetching paranoia mode and\ndefault secuity settings...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					// Fetch Pmode
					authData.paranoia_mode = setupForm.paranoiaMode;
					authData.fail_count = 0;

					infoMsg += "Generating master AES key and IV...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					// Generate AES Key and IV
					PalcomAes aes;
					if(!aes.generate()){
						throw CoreException("PalcomSetup::run() - Failed to generate AES keys.\n", 0x02);
					}
					for(int i=0; i<32; i++){
						this->execute();
						authData.aes_key[i] = aes.key[i];
					}
					for(int i=0; i<16; i++){
						this->execute();
						authData.aes_iv[i] = aes.iv[i];
					}

					this->execute();

					infoMsg += "Generating RSA key pairs...\nThis might take a while.\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					// Store non-sensitive config data (user name, lock timer, etc)
					palcom_config_t configData;
					configData.lock_timer = setupForm.lockTimer;
					for(int i=0; i<setupForm.name.length(); i++){
						this->execute();
						if(i < 20 )
							configData.user_name[i] = setupForm.name[i];
					}
					configData.screen_brightness = 100;
					this->execute();
					pfs.storeConfigData(configData);
					this->execute();

					pcry.rsaKeyGen.deleteKeyPair();
					this->execute();
                        		pcry.rsaKeyGen.initalizeVars();
					this->execute();

                        		infoMsg += "Generating RNG Seed...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                        		if(!pcry.rsaKeyGen.generateRngSeed()){
                        		        return false;
                        		}

					this->execute();
                        		infoMsg += "Generating key pair...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                        		if(!pcry.rsaKeyGen.generateKeyPair()){
                        		        return false;
                        		}
					this->execute();

                        		infoMsg += "Exporting key pair..\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                        		if(!pcry.rsaKeyGen.exportKeys()){
                        		        return false;
                        		}
					this->execute();

                        		infoMsg += "Storing Public Key...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                        		if(!pcry.rsaKeyGen.storeExportedPublicKey()){
                        		        return false;
                        		}

					this->execute();
                        		infoMsg += "Storing private key...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                        		if(!pcry.rsaKeyGen.storeExportedPrivateKey()){
                        		        return false;
                        		}
					this->execute();


                        		infoMsg += "Key Generation Successful!\n";
					lab.setText(infoMsg.c_str());
					this->execute();
                       		 	pcry.rsaKeyGen.freeVars();
					this->execute();
					//if(!pcry.rsaKeyGen.genKeys()){
                                	//	Serial.printf("Failed to generate keys.\n");
                        		//}

					// Generating phone number.
					infoMsg += "Generating Phone Number...\n";
					lab.setText(infoMsg.c_str());
					this->execute();

					pfs.clearFileBuffer();
					this->execute();
					pfs.fd = SD.open(pfs_file_keysPublic, FILE_READ);
					size_t ksize = pfs.fd.size();
					pfs.fd.read((unsigned char *)&fileData, ksize);
					pfs.close();
					this->execute();

					PalcomAlgorithms pa;
					String phoneNum = pa.PalPhoGen7Shrink(fileData, ksize);
					this->execute();
					for(int i=0; i<11; i++){
						this->execute();
						authData.phone_number[i] = phoneNum[i];
					}

					this->execute();
					infoMsg += "Storing security settings.\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					// Store sensitive data in flash memory.
					pp.fetchPartitionByName("app1");
					this->execute();
					pp.writeAuthData((const esp_partition_t *)pp.partition, authData);
					this->execute();
					pp.freePartitions();
					this->execute();
					aes.clearKey();
					this->execute();
					aes.clearIv();
					this->execute();
					
					infoMsg += "Encrypting private key with master key...\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					encryptPrivateKey();
					this->execute();
					

					infoMsg += "\n!SETUP COMPLETE!\n";
					lab.setText(infoMsg.c_str());
					this->execute();
					delay(5000);
                        	        resetPage();
                        	        return 0; // change screen to login page.
				}else{
					this->execute();
				}

			}catch(CoreException e){
				e.log("PalcomSetup::run()");
				throw;
			}	
			
      			return -1; // loop the setup page.
    		}
}palcomSetup;
