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
      settings.setSize(40, 30);
      pLabel.create(settings.getObj());
      pLabel.setText("back");
      pLabel.center();
      settings.setLabel(pLabel);
      settings.setSimpleCallback(Settingsmenu_handleBackButton);
      settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  -20);

      // factory reset button
      settings.create(screen);
      settings.setSize(40, 30);
      pLabel.create(settings.getObj());
      pLabel.setText("reset");
      pLabel.center();
      settings.setLabel(pLabel);
      settings.setSimpleCallback(Settingsmenu_handleResetButton);
      settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  60);
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