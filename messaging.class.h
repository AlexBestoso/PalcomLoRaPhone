int Messaging_pageContext = 0;
lv_obj_t *Messageing_generalRecvText = NULL;
lv_obj_t *Messageing_generalSendText = NULL;

int selectedHash = -1;
int selectedFriend = -1;
int activeTab = 0;
char friendHash[33];

class PalcomMessaging: public PalcomScreen{
private:
  bool buildRequired = true;
  lv_obj_t *tabDescriptor = NULL;
  lv_obj_t *tabOne = NULL;
  lv_obj_t *tabTwo = NULL;
  lv_obj_t *tabThree = NULL;
  lv_obj_t *tabFour = NULL;
  size_t lastPublicSize = 0;
  float msgCheckTimer = millis();

  
  string friendName = "";
  string friendFile = "";

  size_t getPublicMsgSize() {
    msgCheckTimer = millis();
    if (!SD.exists(pfs_public_msg_log))
      return 0;

    File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
    size_t msgFileSize = msgFile.size();
    msgFile.close();
    return msgFileSize;
  }

  void getFriendFile(void){
     if(SD.exists(pfs_dir_friends)){
      int friendCount = 0;
      File root = SD.open(pfs_dir_friends);
      while(true){
        if(friendCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(friendCount == selectedFriend){
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          friendFile = (const char *)fileData;
          node.close();
          break;
        }
        node.close(); 
      }
      root.close();
    }
  }

  void getFriendHash(void){
     if(SD.exists(pfs_dir_friends)){
      int friendCount = 0;
      File root = SD.open(pfs_dir_friends);
      while(true){
        if(friendCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(friendCount == selectedFriend){
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          size_t keySize = node.size();
          node.read((uint8_t *)compBuffer, keySize);
          node.close();

          getSha256Hash((char *)compBuffer, (const size_t)keySize, friendHash);
          break;
        }
        node.close(); 
      }
      root.close();
    }
  }

  void getPublicMsgData(void) {
    string ret = "";
    if (SD.exists(pfs_public_msg_log)) {
      File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
      size_t msgFileSize = msgFile.size();
      for (int i = 0; i < 100000; i++)
        fileData[i] = 0;
      lastPublicSize = msgFileSize;
      msgFile.read(fileData, msgFileSize);
      msgFile.close();
    } else {
      lastPublicSize = 0;
    }
  }

  void getFriendMsgData(void){
    if(selectedFriend <= -1){
      return;
    }

    if (SD.exists(pfs_public_msg_log)) {
      File msgFile = SD.open(pfs_public_msg_log, FILE_READ);
      size_t msgFileSize = msgFile.size();
      for (int i = 0; i < 100000; i++)
        fileData[i] = 0;
      lastPublicSize = msgFileSize;
      msgFile.read(fileData, msgFileSize);
      msgFile.close();
    } else {
      lastPublicSize = 0;
    }
  }

  static void Messaging_handleEncryptedSend(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED || selectedFriend == -1)
        return;
    
    //char  friendHash = "";
    string friendFile = "";
    if(SD.exists(pfs_dir_friends)){
      int friendCount = 0;
      File root = SD.open(pfs_dir_friends);
      while(true){
        if(friendCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(friendCount == selectedFriend){
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          friendFile = (const char *)fileData;
          size_t keySize = node.size();
          node.read((uint8_t *)compBuffer, keySize);
          node.close();

          getSha256Hash((char *)compBuffer, (const size_t)keySize, friendHash);
          break;
        }
        node.close(); 
      }
      root.close();
    }

    PalcomTextarea pTextarea;
    pTextarea.loadGlobal(2);
    string msg = pTextarea.getText();
    if (msg.length() <= 0) {
      Serial.printf("No message, not sending.\n");
      return;
    }

    if(SD.exists(pfs_file_cryptSend)){
      SD.remove(pfs_file_cryptSend);
    }

    rsaEncrypt(friendFile.c_str(), (const unsigned char *)msg.c_str(), msg.length(), pfs_file_cryptSend);
    File encryptedData = SD.open(pfs_file_cryptSend, FILE_READ);
    size_t ctextSize = encryptedData.size();
    encryptedData.read(fileData, ctextSize);
    encryptedData.close();
    for(int i=0; i<33; i++){
      compBuffer[i] = friendHash[i];
    }
    for(int i=0; i<ctextSize; i++){
      compBuffer[i+33] = fileData[i];
    }
    palcomRadio.sendEncryptedMessage((const char *)compBuffer, 33+ctextSize);
    pTextarea.setText("");
  }

  static void Messaging_handleLeaveFriend(lv_event_t *e){
     if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    selectedFriend = -1;
    newPacketReceived = true;
    activeTab = 1;
  }

  static void Messaging_handleUpdateContact(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    bool renamed = false;
    string newName = "";
    
    if(SD.exists(pfs_dir_friends)){
      int friendCount = 0;
      File root = SD.open(pfs_dir_friends);
      while(true){
        if(friendCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(friendCount == selectedFriend){
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          string original = (const char *)fileData;

          PalcomTextarea pText;
          pText.loadGlobal(3);
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, pText.getText());
          string newFile = (const char *)fileData;

          if(newFile != original){
            newName = pText.getText();
            size_t fileSize = node.size();
            node.read(fileData, fileSize);
            node.close();
            node = SD.open(newFile.c_str(), FILE_WRITE);
            node.write(fileData, fileSize);
            node.close();
            SD.remove(original.c_str());
            newPacketReceived = true;
            renamed = true;
            break;
          }
          node.close();
          break;
        }
        node.close(); 
      }
      root.close();
    }

    if(renamed){
      File root = SD.open(pfs_dir_friends);
      int index = 0;
      while(true){
        File node = root.openNextFile();
        if(!node)
          break;
        string test = node.name();
        if(test == newName){
          selectedFriend = index;
          node.close();
          break;
        }
        node.close();
        index++;
      }
      root.close();
    }
  }

  static void Messaging_handleDeleteContact(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    if(SD.exists(pfs_dir_friends)){
      int friendCount = 0;
      File root = SD.open(pfs_dir_friends);
      while(true){
        if(friendCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(friendCount == selectedFriend){
          sprintf((char *)fileData, "%s/%s", pfs_dir_friends, node.name());
          SD.remove((const char *)fileData);
          selectedFriend = -1;
          activeTab = 1;
          newPacketReceived = true;
          node.close();
          break;
        }
        node.close();
      }
      root.close();
    }
  }
  static void Messaging_handleGeneralSend(lv_event_t *e) {
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
        return;
    PalcomTextarea pTextarea;
    pTextarea.loadGlobal(2);
    string msg = pTextarea.getText();
    if (msg.length() <= 0) {
      Serial.printf("No message, not sending.\n");
      return;
    }
    palcomRadio.sendPublicMessage(msg);
    pTextarea.setText("");
  }

  uint8_t hashBuffer[25];
  uint8_t firendBuffer[256];
  static void Messaging_handleReceivedKey(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    uint8_t* val = (uint8_t *)lv_event_get_user_data(e);
    selectedHash = (int)*val;
    newPacketReceived = true;
  }

  static void Messaging_handleSelectedFriend(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    uint8_t* val = (uint8_t *)lv_event_get_user_data(e);
    selectedFriend = (int)*val;
    newPacketReceived = true;
  }

  static void Messaging_handleShareKey(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    palcomRadio.sendFriendKey();
  }

  static void Messaging_handleHomepage(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    Messaging_pageContext = 1;
  }

  static void Messaging_handleApproveKey(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;

    PalcomFS pfs;
    const char *targetKeyName = pfs.getFilenameByPos(selectedHash, pfs_dir_requests);
    if(targetKeyName == NULL){
      return;
    }
    // Read key data     
    sprintf((char*)compBuffer, "%s/%s", pfs_dir_requests, targetKeyName);
    File node = SD.open(compBuffer);      
    size_t keySize = node.size();
    node.read(fileData, keySize);
    node.close();
    SD.remove(compBuffer);

    // Make sure root friends dir exists
    if(!SD.exists(pfs_dir_friends)){
      SD.mkdir(pfs_dir_friends);
    }

    // Make the directory for your new friend.
    sprintf((char *)compBuffer, "%s/%s", pfs_dir_friends, targetKeyName);
    if(!SD.exists(compBuffer)){
      SD.mkdir(compBuffer);
    }

    //Write key
    string n = compBuffer;
    sprintf((char *)compBuffer, "%s/%s", compBuffer, "key");
    node = SD.open((const char *)compBuffer, FILE_WRITE);
    node.write(fileData, keySize);
    node.close();

    // Write Name File
    sprintf((char *)compBuffer, "%s/%s", n.c_str(), "name");
    node = SD.open((const char *)compBuffer, FILE_WRITE);
    node.printf("My Fren");
    node.close();

    activeTab = 2;
    selectedHash = -1;
    newPacketReceived = true;
  }

  static void Messaging_handleDenyKey(lv_event_t *e){
    if (lv_event_get_code(e) != LV_EVENT_CLICKED)
      return;
    if(SD.exists(pfs_dir_requests)){
      int buttonCount = 0;
      File root = SD.open(pfs_dir_requests);
      while(true){
        if(buttonCount > 25)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(buttonCount == selectedHash){
          sprintf((char *)fileData, "%s/%s", pfs_dir_requests, node.name());
          SD.remove((const char *)fileData);
          node.close();
          break;
        }

        node.close(); 
      }
      root.close();
    }
    activeTab = 2;
    selectedHash = -1;
    newPacketReceived = true;
  }

  PalcomTabMenu tabMenu;

  void mainView(){
    lv_obj_t *screen = this->getScreen();
    if(screen == NULL){
      this->globalDestroy();
      this->create();
      screen = this->getScreen();
    }
    this->setFullScreen();
    this->setScreenScrollDirection(LV_DIR_VER);
    PalcomLabel pLabel;
    PalcomButton pButton;
    PalcomTextarea pTextarea;

    tabMenu.create(screen);

    tabMenu.addTab(0, "General");
    tabMenu.addTab(1, "Secure");
    tabMenu.addTab(2, "Key Share");
    tabMenu.addTab(3, "Menu");
    lv_task_handler();

    pTextarea.createGlobal(tabMenu.getTab(0), 1);
    pTextarea.setCursorClickPos(false);
    pTextarea.setTextSelection(false);
    pTextarea.setSize(LV_HOR_RES - 10, (LV_VER_RES / 3) + 20);
    getPublicMsgData();
    if (lastPublicSize <= 0)
      pTextarea.setText("");
    else
      pTextarea.setText((const char *)fileData);
    pTextarea.setAlignment(LV_ALIGN_TOP_MID, 0, 0);
    //lv_obj_add_style(Messageing_generalRecvText, &bg_style, LV_PART_ANY);
    lv_task_handler();

    string retainer = "";
    pTextarea.loadGlobal(2);
    if (pTextarea.getObj() != NULL)
      retainer = pTextarea.getText();
    pTextarea.createGlobal(tabMenu.getTab(0), 2);
    pTextarea.setCursorClickPos(false);
    pTextarea.setTextSelection(false);
    pTextarea.setSize(LV_HOR_RES - 89, (LV_VER_RES / 3) - 20);
    pTextarea.setText(retainer.c_str());
    pTextarea.setAlignment(LV_ALIGN_BOTTOM_MID, ((75 / 2) * -1), 0);
    //lv_obj_add_style(Messageing_generalSendText, &bg_style, LV_PART_ANY);
    lv_task_handler();

    pButton.create(tabMenu.getTab(0));
    pButton.setSize(24, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Send");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleGeneralSend);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 230, 125);
    lv_task_handler();
    
    if(SD.exists(pfs_dir_friends)){
      File root = SD.open(pfs_dir_friends);
      int buttonSpacing = 60;
      int buttonCount = 0;
      while(true){
        if(buttonCount > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        pButton.create(tabMenu.getTab(1));
        pButton.setSize(100, 30);
        pLabel.create(pButton.getObj());
        pLabel.setText(node.name());
        pLabel.center();
        pButton.setLabel(pLabel);
        firendBuffer[buttonCount] = buttonCount;
        pButton.setValuedCallback(Messaging_handleSelectedFriend, &firendBuffer[buttonCount]);
        pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, -5, 60+(buttonSpacing*buttonCount));    
        node.close();
        buttonCount++;
        lv_task_handler(); 
      }
      root.close();
    }
    lv_task_handler(); 
    
    pButton.create(tabMenu.getTab(2));
    pButton.setSize(30, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Share Key");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleShareKey);
    pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 100, 0);
    lv_task_handler();
    if(SD.exists(pfs_dir_requests)){
      File root = SD.open(pfs_dir_requests);
      int buttonSpacing = 60;
      int buttonCount = 0;
      while(true){
        if(buttonCount > 25)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        pButton.create(tabMenu.getTab(2));
        pButton.setSize(100, 30);
        pLabel.create(pButton.getObj());
        pLabel.setText(node.name());
        pLabel.center();
        pButton.setLabel(pLabel);
        hashBuffer[buttonCount] = buttonCount;
        pButton.setValuedCallback(Messaging_handleReceivedKey, &hashBuffer[buttonCount]);
        pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, -5, 60+(buttonSpacing*buttonCount));    
        node.close();
        buttonCount++;
        lv_task_handler(); 
      }
      root.close();
    }
  
    pButton.create(tabMenu.getTab(3));
    pButton.setSize(25, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Home");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleHomepage);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 100, 125);
    lv_task_handler(); 
    if(activeTab){
      tabMenu.setActiveTab(activeTab, false);
      lv_task_handler();
      activeTab = 0;
    }
    lv_task_handler();
  }

  void approvalView(){
    lv_obj_t *screen = this->getScreen();
    if(screen == NULL){
      this->globalDestroy();
      this->create();
      screen = this->getScreen();
    }
    this->setFullScreen();
    this->setScreenScrollDirection(LV_DIR_VER);
    PalcomLabel pLabel;
    PalcomButton pButton;
    PalcomTextarea pTextarea;

    tabMenu.create(screen);
    
    tabMenu.addTab(0, "Key Share");
    
    pButton.create(tabMenu.getTab(0));
    pButton.setSize(30, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Approve");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleApproveKey);
    pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 25, 0);

    pButton.create(tabMenu.getTab(0)); 
    pButton.setSize(30, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Deny");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleDenyKey);
    pButton.setRelativeAlignment(LV_ALIGN_TOP_MID, 150, 0);
    lv_task_handler();
    return;
  }

  void friendView(){
    lv_obj_t *screen = this->getScreen();
    if(screen == NULL){
      this->globalDestroy();
      this->create();
      screen = this->getScreen();
    }
    this->setFullScreen();
    this->setScreenScrollDirection(LV_DIR_VER);

    PalcomLabel pLabel;
    PalcomButton pButton;
    PalcomTextarea pTextarea;

    tabMenu.create(screen);
    lv_task_handler();

    tabMenu.addTab(0, "Messages");
    tabMenu.addTab(1, "Contact Info");
    tabMenu.addTab(3, "Menu");
    lv_task_handler();

    pTextarea.createGlobal(tabMenu.getTab(0), 1);
    pTextarea.setCursorClickPos(false);
    pTextarea.setTextSelection(false);
    pTextarea.setSize(LV_HOR_RES - 10, (LV_VER_RES / 3) + 20);
    getPublicMsgData();
    if (lastPublicSize <= 0)
      pTextarea.setText("");
    else
      pTextarea.setText((const char *)fileData);
    pTextarea.setAlignment(LV_ALIGN_TOP_MID, 0, 0);
    //lv_obj_add_style(Messageing_generalRecvText, &bg_style, LV_PART_ANY);
    lv_task_handler();

    string retainer = "";
    pTextarea.loadGlobal(2);
    if (pTextarea.getObj() != NULL)
      retainer = pTextarea.getText();
    pTextarea.createGlobal(tabMenu.getTab(0), 2);
    pTextarea.setCursorClickPos(false);
    pTextarea.setTextSelection(false);
    pTextarea.setSize(LV_HOR_RES - 89, (LV_VER_RES / 3) - 20);
    pTextarea.setText(retainer.c_str());
    pTextarea.setAlignment(LV_ALIGN_BOTTOM_MID, ((75 / 2) * -1), 0);
    //lv_obj_add_style(Messageing_generalSendText, &bg_style, LV_PART_ANY);
    lv_task_handler();

    pButton.create(tabMenu.getTab(0));
    pButton.setSize(24, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Send");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleEncryptedSend);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 230, 125);
    lv_task_handler();

    pLabel.create(tabMenu.getTab(1));
    pLabel.setLongMode(LV_LABEL_LONG_SCROLL);
    pLabel.setWidth(320);
    pLabel.setAlignment(LV_ALIGN_TOP_MID, 5, 10);
    pLabel.setText("Name:");

    pTextarea.createGlobal(tabMenu.getTab(1), 3);
    pTextarea.setCursorClickPos(false);
    pTextarea.setTextSelection(false);
    pTextarea.setSize(220, 23);
    pTextarea.setMaxLength(33);
    pTextarea.setOneLine(true);
    pTextarea.setAlignment(LV_ALIGN_TOP_MID, 15, 4);
    lv_task_handler();
    if(SD.exists(pfs_dir_friends)){
      File root = SD.open(pfs_dir_friends);
      int index = 0;
      while(true){
        if(index > 256)
          break;
        File node = root.openNextFile();
        if(!node)
          break;
        if(index == selectedFriend){
          pTextarea.setText(node.name());
          node.close();
          break;
        }
        node.close();
        index++;
        lv_task_handler(); 
      }
      root.close();
    }
    lv_task_handler();

    pButton.create(tabMenu.getTab(1));
    pButton.setSize(25, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Update");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleUpdateContact);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 50, 125);
    lv_task_handler();

    pButton.create(tabMenu.getTab(1));
    pButton.setSize(25, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Delete");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleDeleteContact);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 150, 125);
    lv_task_handler();

    pButton.create(tabMenu.getTab(3));
    pButton.setSize(25, 30);
    pLabel.create(pButton.getObj());
    pLabel.setText("Back");
    pLabel.center();
    pButton.setLabel(pLabel);
    pButton.setSimpleCallback(Messaging_handleLeaveFriend);
    pButton.setRelativeAlignment(LV_ALIGN_BOTTOM_MID, 100, 125);
    lv_task_handler();
  }
public:
  PalcomMessaging(){
    selectedHash = -1;
  }
  void resetPage(void) {
    buildRequired = true;
    Messaging_pageContext = 0;
    selectedFriend = -1;
    selectedHash = -1;
    this->globalDestroy();
    this->destroy();
  }

  void generateObjects(void){
    if(selectedFriend != -1){
      this->friendView();
      lv_task_handler();
      return;
    }
    
    if(selectedHash != -1){
      this->approvalView();
      lv_task_handler();
      return;
    }
    
    this->mainView();
    lv_task_handler();
  }
  int run(void) {
    if (buildRequired) {
      this->load();
      buildRequired = false;
    }
    if(selectedHash <= -1){
      activeTab = tabMenu.getActiveTab();
    }
    lv_task_handler();

    if (Messaging_pageContext <= 0) {
      if ((millis() - msgCheckTimer) > ((1000 * 60)) || newPacketReceived) {
        if (getPublicMsgSize() != lastPublicSize || newPacketReceived) {
          this->globalDestroy();
          this->destroy();
          lv_task_handler();
          buildRequired = true;
        }
        newPacketReceived = false;
        msgCheckTimer = millis();
      }
    }
    if(Messaging_pageContext == 1){
      resetPage();
      return 1;
    }
    return 3;
  }
} palcomMessaging;