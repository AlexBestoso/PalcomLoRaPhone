class PalcomCore{
  	private:
		int viewContext = -1;
    		lv_obj_t *parent;
    		PalcomLoginScreen loginScreen;    

		void execute(void){
			lv_task_handler();
                        lv_timer_handler();
                        lv_tick_inc(5);
		}

		void updateSleepTimer(void){
			PalcomFS pfs;
			this->execute();
			palcom_config_t *cfg = pfs.getConfigData();
			this->execute();
			screenSleepTime = cfg->lock_timer;
			this->execute();

		}

		void updateBrightness(void){
			PalcomFS pfs;
			this->execute();
			palcom_config_t *cfg = pfs.getConfigData();
			this->execute();
			screenBrightness = cfg->screen_brightness;
			this->execute();
			analogWrite(BOARD_TFT_BACKLIGHT, screenBrightness);
		}

		void updateLockout(void){
			PalcomPartition pp;
			pp.fetchPartitionByName("app1");
			palcom_auth_t data;
			pp.readAuthData((const esp_partition_t *)pp.partition, &data);
			if(data.fail_count >= 3){
				loginScreen.setLockout(data.fail_count);
			}
		}

    		void _resetAllPages(int maintain=0){
      			viewContext = maintain;
			aboutScreen.resetPage();
      			loginScreen.resetPage();
      			palcomSetup.resetPage();
      			mainMenu.resetPage();
			this->execute();

      			palcomPlaintextMessaging.resetPage();
      			settingsMenu.resetPage();
      			palcomKeySharing.resetPage();
      			palcomEncryptedMessaging.resetPage();
			wifiMenu.resetPage();
      			this->execute();
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
			if(settingsMenu.updateTimer())
				updateSleepTimer();
			if(settingsMenu.updateBrightness())
				updateBrightness();
      			if(viewContext != CONTEXT_SETTINGS){
        			_resetAllPages(viewContext);
      			}
			//if(viewContext == CONTEXT_WIFI){
			//	wifiMenu.setAlternateReturn(CONTEXT_SETTINGS);
		//	}
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
		int screenSleepTime = 3;
		int screenBrightness = Sleep_maxBrightness;
		
    		void screenSleep(){
			if(screenSleepTime <= 0){
				return;
			}
      			if(Sleep_interactionCtx == 0 && Sleep_brightness != screenBrightness){
        			Sleep_brightness = screenBrightness;
        			Sleep_timer = millis();
        			analogWrite(BOARD_TFT_BACKLIGHT, screenBrightness);
      			}else if((millis()-Sleep_timer) > (1000*60*screenSleepTime) && Sleep_interactionCtx == 0){
        			Sleep_interactionCtx = 1;
        			Sleep_timer = millis();
        			Sleep_brightness = screenBrightness/2;
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
			updateSleepTimer();
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
					this->execute();
        				if(sendTimer > 0){
        	  				if(this->_processRecv())
        	    					sendTimer = 0;
						this->execute();
        				}else if(sendTimer <= 0){
        	  				this->_processSend();
						this->execute();
        	  				sendTimer = sendTimerMax;
        				}
      				}else{
        				delay(1);
					this->execute();
      				}

      				switch(viewContext){
        				case CONTEXT_SETUP:
						viewContext = palcomSetup.run();
                        			if(viewContext != -1){
							updateSleepTimer();
							updateBrightness();
							updateLockout();
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
				this->execute();
			}catch(CoreException e){
				e.log("PalcomCore::contextSwitch() - Critical Failure.");
				while(1){}
			}
    		}
}palcomCore;
