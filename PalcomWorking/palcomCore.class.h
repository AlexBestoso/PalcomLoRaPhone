class PalcomCore{
  private:
    int viewContext = -1;
    lv_obj_t *parent;
    PalcomLoginScreen loginScreen;    

    void _resetAllPages(int maintain=0){
      viewContext = maintain;
      palcomMessaging.resetPage();
      settingsMenu.resetPage();
      loginScreen.resetPage();
      palcomSetup.resetPage();
      mainMenu.resetPage();
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
        viewContext = 1;
        _resetAllPages(viewContext);
      }
    }

    void _mainMenu(void){
      viewContext = mainMenu.run();
      if(viewContext != 1){
        _resetAllPages(viewContext);
      }
    }

    void  _messageMenu(void){
      viewContext = palcomMessaging.run();
      if(viewContext != 3){
         _resetAllPages(viewContext);
      }
    }

    void _settingsMenu(void){
      viewContext = settingsMenu.run();
      if(viewContext != 2){
        _resetAllPages(viewContext);
      }
    }

  public:
    void initSystem(){
      viewContext = palcomSetup.run();
      if(viewContext != -1){
        _resetAllPages(viewContext);
        PalcomFS pfs;
        pfs.rm(pfs_folder_recvQueue);
        pfs.rm(pfs_folder_sendQueue);
      }
    }

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

    int sendTimerMax = 20000;
    int sendTimer = sendTimerMax;
    void contextSwitch(void){
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
        case -1:
          this->initSystem();
          break;
        case 3:
          this->_messageMenu();
          break;
        case 2:
          this->_settingsMenu();
          break;
        case 1:
          this->_mainMenu();
          break;
        default:
          this->_login();
          break;
      }
      lv_task_handler();
    }
}palcomCore;