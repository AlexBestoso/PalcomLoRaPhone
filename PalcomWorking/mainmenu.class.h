int mainMenu_contextControl = 0;

class PalcomMainMenu : public PalcomScreen{
  private:
    static void Mainmenu_handleLogoutButton(lv_event_t *e){
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      mainMenu_contextControl = 1;
    }

    static void Mainmenu_handleSettingsButton(lv_event_t *e){
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      mainMenu_contextControl = 2;
    }

    static void Mainmenu_handleMessagesButton(lv_event_t *e){
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      mainMenu_contextControl = 3;
    }
  public:
  bool buildRequired = true;
  
  void generateObjects(void){
    // Configure Screen
    lv_obj_t *screen = this->getScreen();
    if(screen == NULL){
      this->globalDestroy();
      this->create();
      screen = this->getScreen();
    }
    this->setFullScreen();
    this->setScreenScrollDirection(LV_DIR_VER);
    
    const lv_img_dsc_t *img_src[1] = {&tiddies_scaled};
    this->setBgImage(img_src);

    PalcomLabel pLabel;

    // Create Settings Button
    PalcomButton settings;
    settings.create(screen);
    settings.setSize(40, 30);
    pLabel.create(settings.getObj());
    pLabel.setText("Settings");
    pLabel.center();
    settings.setLabel(pLabel);
    settings.setSimpleCallback(Mainmenu_handleSettingsButton);
    settings.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  -20);

    PalcomButton messages;
    messages.create(screen);
    messages.setSize(40, 30);
    pLabel.create(messages.getObj());
    pLabel.setText("Messages");
    pLabel.center();
    messages.setLabel(pLabel);
    messages.setSimpleCallback(Mainmenu_handleMessagesButton);
    messages.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10,  50);

    // Create Logout Button
    PalcomButton logout;
    logout.create(screen);
    logout.setSize(40, 30);
    pLabel.create(logout.getObj());
    pLabel.setText("Logout");
    pLabel.center();
    logout.setLabel(pLabel);
    logout.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, 10, 120);
    logout.setSimpleCallback(Mainmenu_handleLogoutButton);

    /*messageInfo = lv_textarea_create(parent);
    lv_textarea_set_cursor_click_pos(messageInfo, false);
    lv_textarea_set_text_selection(messageInfo, false);
    lv_obj_set_size(messageInfo, 160, 200);
    lv_textarea_set_text(messageInfo, "Unread:\n\t0\n\nFriends:\n\t0\n\nMy Alias:\n\tMorningstar\n\nRelay Mode:\n\tfalse");
    lv_textarea_set_max_length(messageInfo, 400);
    lv_textarea_set_one_line(messageInfo, false);
    lv_textarea_set_password_mode(messageInfo, false);
    lv_obj_align(messageInfo, LV_ALIGN_TOP_MID, 70, 20);
    lv_obj_add_style(messageInfo, &inputStyle, LV_PART_ANY);*/
  }
  void resetPage(){
    buildRequired = true;
    mainMenu_contextControl = 0;
    this->globalDestroy();
    this->destroy();
  }
  int run(){
    if(buildRequired){
      buildRequired = false;
      mainMenu_contextControl = 0;
      this->load();
    }
    
    lv_task_handler();

    if(mainMenu_contextControl == 1){
      this->globalDestroy();
      this->destroy();
      buildRequired = true;
      return 0; // logout
    }

    if(mainMenu_contextControl == 2){
      this->globalDestroy();
      this->destroy();
      buildRequired = true;
      return 2; // Settings
    }

    if(mainMenu_contextControl == 3){
      this->globalDestroy();
      this->destroy();
      buildRequired = true;
      return 3; // Messages
    }
    return 1;
  }
}mainMenu;