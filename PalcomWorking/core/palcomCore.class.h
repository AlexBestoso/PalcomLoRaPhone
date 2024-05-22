class PalcomCore{
  	private:
		int viewContext = -1;
    		lv_obj_t *parent;
    		PalcomLoginScreen loginScreen;    

    		void _resetAllPages(int maintain=0){
      			viewContext = maintain;
			aboutScreen.resetPage();
      			loginScreen.resetPage();
      			palcomSetup.resetPage();
      			mainMenu.resetPage();

      			palcomPlaintextMessaging.resetPage();
      			settingsMenu.resetPage();
      			palcomKeySharing.resetPage();
      			palcomEncryptedMessaging.resetPage();
			wifiMenu.resetPage();
      			lv_task_handler();
    		}

    		bool _processRecv(){
      			lv_task_handler();
      			return palcomRadio.recvMessage();
    		}

    		void _processSend(){
      			lv_task_handler();
      			palcomRadio.sendQueue();
    		}

    		void _login(void){
      			if(loginScreen.run()){
        			viewContext = CONTEXT_MAINMENU;
        			_resetAllPages(viewContext);
      			}
    		}

		void _aboutScreen(void){
			if((viewContext = aboutScreen.run()) != CONTEXT_ABOUT){
				_resetAllPages(viewContext);
			}
		}

    		void _mainMenu(void){
      			viewContext = mainMenu.run();
      			if(viewContext != CONTEXT_MAINMENU){
        			_resetAllPages(viewContext);
      			}
    		}

    		void  _plaintextMessageMenu(void){
      			viewContext = palcomPlaintextMessaging.run();
      			if(viewContext != CONTEXT_PLAINTEXT_MESSAGING){
        			_resetAllPages(viewContext);
      			}
    		}

    		void _encryptedMessageMenu(void){
      			viewContext = palcomEncryptedMessaging.run();
      			if(viewContext != CONTEXT_CIPHERTEXT_MESSAGING){
        			_resetAllPages(viewContext);
      			}
    		}

    		void _keyShareMenu(void){
      			viewContext = palcomKeySharing.run();
      			if(viewContext != CONTEXT_KEYSHARING){
        			_resetAllPages(viewContext);
      			}
    		}

    		void _settingsMenu(void){
      			viewContext = settingsMenu.run();
      			if(viewContext != CONTEXT_SETTINGS){
        			_resetAllPages(viewContext);
      			}
			if(viewContext == CONTEXT_WIFI){
				wifiMenu.setAlternateReturn(CONTEXT_SETTINGS);
			}
    		}

		void _wifiSettingsiMenu(void){
			viewContext = wifiMenu.run();
			if(viewContext != CONTEXT_WIFI){
				_resetAllPages(viewContext);
			}
		}

  	public:
    		int sendTimerMax = 20000;
    		int sendTimer = sendTimerMax;
		
    		void screenSleep(){
      			if(Sleep_interactionCtx == 0 && Sleep_brightness != Sleep_maxBrightness){
        			Sleep_brightness = Sleep_maxBrightness;
        			Sleep_timer = millis();
        			analogWrite(BOARD_TFT_BACKLIGHT, Sleep_maxBrightness);
      			}else if((millis()-Sleep_timer) > (1000*60) && Sleep_interactionCtx == 0){
        			Sleep_interactionCtx = 1;
        			Sleep_timer = millis();
        			Sleep_brightness = Sleep_maxBrightness/2;
        			analogWrite(BOARD_TFT_BACKLIGHT, Sleep_brightness);
      			}else if((millis()-Sleep_timer) > (1000*30) && Sleep_interactionCtx == 1){
        			Sleep_interactionCtx = 2;
        			Sleep_timer = millis();
        			Sleep_brightness = 0;
        			viewContext = (viewContext == -1) ? -1 : 0;
        			analogWrite(BOARD_TFT_BACKLIGHT, 0);
        			_resetAllPages();
      			}
    		}

    		void lockScreen(){
      			screenLockConditionSpace = false;
      			screenLockConditionBall = false;
      			Sleep_interactionCtx = 2;
      			Sleep_timer = millis();
      			Sleep_brightness = 0;
      			viewContext = (viewContext == -1) ? -1 : 0;
      			analogWrite(BOARD_TFT_BACKLIGHT, 0);
      			_resetAllPages();
    		}

    		
		void contextSwitch(void){
			try{
      				if(screenLockConditionBall && screenLockConditionSpace)
        				this->lockScreen();
      	
      				if(viewContext != -1){
     	 	  			sendTimer--;
      		  			this->screenSleep();
      		  			lv_task_handler();
        				if(sendTimer > 0){
        	  				if(this->_processRecv())
        	    					sendTimer = 0;
        	  				lv_task_handler();
        				}else if(sendTimer <= 0){
        	  				this->_processSend();
        	  				lv_task_handler();
        	  				sendTimer = sendTimerMax;
        				}
      				}else{
        				delay(1);
        				lv_task_handler();
      				}

      				switch(viewContext){
        				case CONTEXT_SETUP:
						viewContext = palcomSetup.run();
                        			if(viewContext != -1){
                                			_resetAllPages(viewContext);
                                			PalcomFS pfs;
                                			pfs.rm(pfs_folder_recvQueue);
                                			pfs.rm(pfs_folder_sendQueue);
                        			}
        	  				break;
       		 			case CONTEXT_MAINMENU:
        	  				this->_mainMenu();
        	  				break;
        				case CONTEXT_SETTINGS:
        	  				this->_settingsMenu();
        	  				break;
					case CONTEXT_CONTACTS:
						
						break;
					case CONTEXT_ABOUT:
						this->_aboutScreen();
						break;	
        				default:
        	  				this->_login();
        					break;
      				}
      				lv_task_handler();
			}catch(CoreException e){
				e.log("PalcomCore::contextSwitch()");
			}
    		}
}palcomCore;
