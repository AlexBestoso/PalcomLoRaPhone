#include "setup.class.h"
#include "radio.class.h"
#include "mainmenu.class.h"
#include "messaging.class.h"
#include "loginScreen.h"

class PalcomCore{
  private:
    int viewContext = 0;
    lv_obj_t *parent;
    PalcomLoginScreen loginScreen;    

    void _resetAllPages(int maintain=0){
      viewContext = maintain;
      palcomMessaging.resetPage();
    }

    void _processRecv(){
      palcomRadio.recvMessage();
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

  public:
    void initSystem(){
      palcomSetup.systemInit();
      parent = lv_scr_act();
      if (!SD.exists(F("/login.hash"))){
        palcomSetup.systemSetup(parent);
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
        viewContext = 0;
        analogWrite(BOARD_TFT_BACKLIGHT, 0);
        _resetAllPages();
      }
    }

    void contextSwitch(void){
      this->screenSleep();
      this->_processRecv();
      switch(viewContext){
        case 3:
          this->_messageMenu();
          break;
        case 2:
          break;
        case 1:
          this->_mainMenu();
          break;
        default:
          this->_login();
          break;
      }
    }

}palcomCore;