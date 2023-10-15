int settings_context = 0;

class PalcomSettingsMenu : public PalcomScreen{
  private:
    static void Settingsmenu_handleBackButton(lv_event_t *e){
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      settings_context = 1;
    }
    static void Settingsmenu_handleResetButton(lv_event_t *e){
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      settings_context = -1;
      PalcomFS pfs;
      pfs.rm("/");
    }
    static void Settingsmenu_setCallsign(lv_event_t *e){
      if(lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      
      PalcomFS pfs;
      PalcomTextarea callsign;
      callsign.loadGlobal(1);

      pfs.setCallsign(callsign.getText());
    }
    bool buildRequired = true;
  public:
    void generateObjects(void){
      lv_obj_t *screen = this->getScreen();
      if(screen == NULL){
        this->globalDestroy();
        this->create();
        screen = this->getScreen();
      }
      this->setFullScreen();
      this->setScreenScrollDirection(LV_DIR_VER);

      PalcomLabel pLabel;
      PalcomButton settings;
      settings.create(screen);
      settings.setSize(40, 25);
      pLabel.create(settings.getObj());
      pLabel.setText("back");
      pLabel.center();
      settings.setLabel(pLabel);
      settings.setSimpleCallback(Settingsmenu_handleBackButton);
      settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  -20);

      // factory reset button
      settings.create(screen);
      settings.setSize(40, 25);
      pLabel.create(settings.getObj());
      pLabel.setText("reset");
      pLabel.center();
      settings.setLabel(pLabel);
      settings.setSimpleCallback(Settingsmenu_handleResetButton);
      settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  100);

      PalcomFS pfs;
      // Callsign input
      pLabel.create(screen);
      pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      pLabel.setWidth(320);
      pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 90);
      pLabel.setText("Callsign: ");

      // callsign Textarea
      PalcomTextarea callsign;
      callsign.createGlobal(screen, 1);
      callsign.setCursorClickPos(false);
      callsign.setTextSelection(false);
      callsign.setSize(150, 23);
      callsign.setText(pfs.getCallsign());
      callsign.setMaxLength(18);
      callsign.setOneLine(true);
      callsign.setAlignment(LV_ALIGN_TOP_MID, 15, 83);

      settings.create(screen);
      settings.setSize(20, 12);
      pLabel.create(settings.getObj());
      pLabel.setText("Update");
      pLabel.center();
      settings.setLabel(pLabel);
      settings.setSimpleCallback(Settingsmenu_setCallsign);
      settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 250,  75);
    }

    void resetPage(void){
      buildRequired = true;
      settings_context = 0;
      this->globalDestroy();
      this->destroy();
    }

    int run(void){
      if(buildRequired){
        buildRequired = false;
        settings_context = 2;
        this->load();
      }

      lv_task_handler();
      return settings_context;
    }
}settingsMenu;