// Call back variables
int login_context = 0;

class PalcomLoginScreen : public PalcomScreen{
  private:
    bool buildRequired = true;

    static void Login_handleSubmit(lv_event_t *e){      
      if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
      PalcomTextarea login_user;
      PalcomTextarea login_pass;
      login_user.loadGlobal(1);
      login_pass.loadGlobal(2);
      string username = login_user.getText();
      string password = login_pass.getText();
      
      if((username == "" || password == "")){
        login_context = 0;
        return;
      }
      char pass_hash[33];
      getSha256Hash((char*)password.c_str(), password.length(), pass_hash);
      
      File hashFile = SD.open("/login.hash", FILE_READ);
      if(!hashFile){
        Serial.println("Failed to open file.");
        while(1){delay(5);}
      }
      size_t hashFileSize = hashFile.size();
      for(int i=0; i<100000; i++)
        fileData[i] = 0;
      hashFile.read(fileData, hashFileSize);
      hashFile.close();

      int iter = 0;
      for(int i=0; i<100000; i++)
        compBuffer[i] = 0;
      for(int i=0; i<username.length(); i++){
        compBuffer[iter] = username[i];
        iter++;
      }
      compBuffer[iter] = '\r';
      iter++;
      compBuffer[iter] = '\n';
      iter++;
      for(int i=0; i<33; i++){
        compBuffer[iter] = pass_hash[i];
        iter++;
      }

      if(strcmp(compBuffer, (const char *)fileData)){
          login_context = 2;
          return;
      }
      login_context = 1;
    }
  public:
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

      // Title Message
      PalcomLabel pLabel;
      pLabel.create(screen);
      pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      pLabel.setWidth(320);
      pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 10);
      if(login_context == 0){
        pLabel.setText("Enter Login");
      }else if(login_context == 3){ 
        pLabel.setText("Logged Out");
      }else{
        pLabel.setText("Invalid Credentials");
      }

      // Username Label
      pLabel.create(screen);
      pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      pLabel.setWidth(320);
      pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 35);
      pLabel.setText("Username: ");

      // Password Label
      pLabel.create(screen);
      pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
      pLabel.setWidth(320);
      pLabel.setAlignment(LV_ALIGN_TOP_MID, 0, 80);
      pLabel.setText("Password: ");

      // Username Textarea
      PalcomTextarea username;
      username.createGlobal(screen, 1);
      username.setCursorClickPos(false);
      username.setTextSelection(false);
      username.setSize(175, 23);
      username.setText("");
      username.setMaxLength(18);
      username.setOneLine(true);
      username.setAlignment(LV_ALIGN_TOP_MID, 20, 30);
      //lv_obj_add_style(Setup_usernameTxt, &ta_bg_style, LV_PART_ANY);

      // Password Textarea
      PalcomTextarea password;
      password.createGlobal(screen, 2);
      password.setCursorClickPos(false);
      password.setTextSelection(false);
      password.setSize(175, 23);
      password.setText("");
      password.setMaxLength(18);
      password.setOneLine(true);
      password.setPasswordMode(true);
      password.setAlignment(LV_ALIGN_TOP_MID, 20, 70);
      //lv_obj_add_style(Setup_passwordTxt, &ta_bg_style, LV_PART_ANY);

      // Submit Button
      PalcomButton submit;
      submit.create(screen);
      submit.setSize(100, 40);
      pLabel.create(submit.getObj());
      pLabel.setText("Login");
      pLabel.center();
      submit.setLabel(pLabel);
      submit.setRelativeAlignment(LV_ALIGN_OUT_BOTTOM_MID, -5, 65);
      submit.setSimpleCallback(Login_handleSubmit);
    }

    bool run(void){
      if(buildRequired){
        buildRequired = false;
        this->load();
      }

      login_context = 0;
      
      lv_task_handler();
      if(login_context == 2){
        this->globalDestroy();
        this->destroy();
        buildRequired = true;
        return false;
      }
      if(login_context == 1){
        this->globalDestroy();
        this->destroy();
        buildRequired = true;
        login_context = 3;
        return true;
      }
      return false;
    }
};