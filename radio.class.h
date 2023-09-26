#define __RECV_CODE_PUBLIC 0
#define __RECV_CODE_KEYSHARE 1
#define __RECV_CODE_ENCRYPTED 2

typedef struct palcomPacket{
  uint32_t p_code;        // Message code plus null byte. 
  uint8_t p_id;           // Ranomly generated message ID.
  uint8_t p_count;        // Number of frames, 0 if one packet.
  uint8_t p_size;         // Number of valid bytes in p_content.
  uint8_t p_content[250]; // The buffer for the actual emitted message
}palcom_packet_t;

bool newPacketReceived = false;

class PalcomRadio{
  private:
    bool listening = true;
    char publicCode[3] = {'\x33', '\x44', '\x55'};
    uint32_t publicCode_int = 0x33445500;
    char keyshareCode[3] = {'\xfa', '\xaf', '\x32'};
    uint32_t keyshareCode_int = 0xfaaf3200;
    char encryptedCode[3] = {'\xe9', '\xc7', '\x49'};
    uint32_t encryptedCode_int = 0xe9c74900;

    uint8_t emitBuffer[256] = {0};
    char tmpName[128];
    char shaHash[33];
    palcom_packet_t *radioPacket;

    void resetRadioPacket(){
      if(radioPacket == NULL)
        return;
      radioPacket->p_code = 0;
      radioPacket->p_id = 0;
      radioPacket->p_count = 0;
      radioPacket->p_size = 0;
      for(int i=0; i<250; i++)
        radioPacket->p_content[i] = 0;
    }

    size_t appendToFile(const char *target, uint8_t * buf, size_t bufSize, bool addUser=false, bool appendNewline=false){
      File targetFd;
      size_t targetSize = 0;
      if(!SD.exists(target)){
        targetFd = SD.open(target, FILE_WRITE);
      }else{
        targetFd = SD.open(target, FILE_READ);
        targetSize = targetFd.size();
        targetFd.read(fileData, targetSize);
        targetFd.close();
        targetFd = SD.open(target, FILE_WRITE, O_TRUNC);
      }

      if(targetSize > 0){
        targetFd.write(fileData, targetSize);
      }
      if(addUser){
        targetFd.printf("[Anon] ");
      }
      targetFd.write(buf, bufSize);
      if(appendNewline)
        targetFd.printf("\r\n");
      targetFd.close();
      targetFd = SD.open(target, FILE_READ);
      targetSize = targetFd.size();
      targetFd.close();
      return targetSize;
    }

    void processKeyshareMessage(){
      if(radioPacket == NULL){
        return;
      }
      if(!SD.exists(pfs_dir_tmp)){
        SD.mkdir(pfs_dir_tmp);
      }
      size_t msgSize = 0;
      sprintf(tmpName, "%s/tmp.%d", pfs_dir_tmp, radioPacket[0].p_id);
      if(radioPacket[0].p_count > 1){
        appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        return;
      }else if(SD.exists(tmpName)){
        msgSize = appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        File tmpFile = SD.open(tmpName, FILE_READ);
        tmpFile.read((uint8_t *)compBuffer, msgSize);
        tmpFile.close();
        SD.remove(tmpName);
      }

      getSha256Hash(compBuffer, msgSize, shaHash);
      sprintf(tmpName, "%s/%s", pfs_dir_requests, shaHash);
      if(SD.exists(tmpName)){
        return;
      }
      File hashFile = SD.open(tmpName, FILE_WRITE, O_TRUNC);
      hashFile.write((uint8_t *)compBuffer, msgSize);
      hashFile.close();
      Serial.printf("Received and stored a public key : (%ld) %s\n", msgSize, shaHash);
      newPacketReceived = true;
    }

    void processEncryptedMessage(void){
      if(radioPacket == NULL){
        return;
      }
      if(!SD.exists(pfs_dir_tmp)){
        SD.mkdir(pfs_dir_tmp);
      }

      // Handle Multi framed packet
      size_t msgSize = 0;
      sprintf(tmpName, "%s/enc.%d", pfs_dir_tmp, radioPacket[0].p_id);
      if(radioPacket[0].p_count > 1){
        appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, (!SD.exists(tmpName)) ? true : false, false);
        return;
      }else if(SD.exists(tmpName)){
        msgSize = appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, true);
        File tmpFile = SD.open(tmpName, FILE_READ);
        tmpFile.read((uint8_t *)fileData, msgSize);
        tmpFile.close();
        SD.remove(tmpName);
      }

      // Write packet to msgLog
      if(!SD.exists(pfs_file_publicLog)){
        if(msgSize > 0) // Multi frame packet
          appendToFile(pfs_file_publicLog, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(pfs_file_publicLog, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }else{
        File msgFile = SD.open(pfs_file_publicLog, FILE_READ);
        size_t msgFileSize = msgFile.size();
        msgFile.close();
        if(msgFileSize > 100000)
          SD.remove(pfs_file_publicLog);

        if(msgSize > 0)
          appendToFile(pfs_file_publicLog, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(pfs_file_publicLog, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }
    }

    void processPublicMessage(void){
      if(radioPacket == NULL){
        return;
      }
      if(!SD.exists(pfs_dir_public)){
        SD.mkdir(pfs_dir_public);
      }

      // Handle Multi framed packet
      size_t msgSize = 0;
      sprintf(tmpName, "%s/tmp.%d", pfs_dir_tmp, radioPacket[0].p_id);
      if(radioPacket[0].p_count > 1){
        appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, (!SD.exists(tmpName)) ? true : false, false);
        return;
      }else if(SD.exists(tmpName)){
        msgSize = appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, true);
        File tmpFile = SD.open(tmpName, FILE_READ);
        tmpFile.read((uint8_t *)compBuffer, msgSize);
        tmpFile.close();
        SD.remove(tmpName);
      }

      // Write packet to msgLog
      if(!SD.exists(pfs_file_publicLog)){
        if(msgSize > 0) // Multi frame packet
          appendToFile(pfs_file_publicLog, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(pfs_file_publicLog, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }else{
        File msgFile = SD.open(pfs_file_publicLog, FILE_READ);
        size_t msgFileSize = msgFile.size();
        msgFile.close();
        if(msgFileSize > 100000)
          SD.remove(pfs_file_publicLog);

        if(msgSize > 0)
          appendToFile(pfs_file_publicLog, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(pfs_file_publicLog, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }
    }

    int parseCode(uint32_t code){
      if(code == publicCode_int){
        return __RECV_CODE_PUBLIC;
      }
      if(code == keyshareCode_int){
        return __RECV_CODE_KEYSHARE;
      }
      if(code == __RECV_CODE_ENCRYPTED){
        return __RECV_CODE_ENCRYPTED;
      }
      return 37707;
    }

    void emitFrames(char *msg, size_t msgSize){
      size_t emitSize = 7;
      if(emitBuffer[5] == 1){
        for(int i=0; i<msgSize && (i+7)<256; i++){
          emitBuffer[i+7] = msg[i];
          emitSize++;
        }
        sendMessage(emitBuffer, emitSize);
      }else{
        bool sendRequired = false;
        for(int i=0; i<msgSize; i++){
          emitBuffer[emitSize] = msg[i];
          emitSize++;
          sendRequired = true;
          if(emitSize == 255){
            emitBuffer[6] = emitSize-7;
            sendMessage(emitBuffer, emitSize);
            delay(200);
            emitSize = 7;
            emitBuffer[5]--;
            sendRequired = false;
          }
        }

        if(sendRequired){
          emitBuffer[6] = emitSize-7;
          sendMessage(emitBuffer, emitSize);
        }
      }
    }
  public:
  PalcomRadio(){
    radioPacket = (palcom_packet_t*)malloc(sizeof(palcom_packet_t));
    radioPacket = (palcom_packet_t*)emitBuffer;
  }
  
  void sendEncryptedMessage(const char *msg, size_t msgSize){
    uint8_t frameCount = 0;
    for(int i=0; i<msgSize; i++){
      frameCount++;
      i=250*frameCount;
    }

    resetRadioPacket();
    for(int i=0; i<256; i++)
      emitBuffer[i] = 0;
    // ESP32 is little-endian
    emitBuffer[0] = 0x00;
    emitBuffer[1] = encryptedCode[2];
    emitBuffer[2] = encryptedCode[1];
    emitBuffer[3] = encryptedCode[0];
    
    emitBuffer[4] = (uint8_t)random(256);
    emitBuffer[5] = frameCount;
    emitBuffer[6] = (uint8_t)((msgSize > 250) ? 250 : msgSize);
    emitFrames((char*)msg, msgSize);
  }

  // TODO: Make it so that these functions don't use strings.
  void sendPublicMessage(string msg){
    uint8_t frameCount = 0;
    for(int i=0; i<msg.length(); i++){
      frameCount++;
      i=250*frameCount;
    }

    resetRadioPacket();
    for(int i=0; i<256; i++)
      emitBuffer[i] = 0;
    // ESP32 is little-endian
    emitBuffer[0] = 0x00;
    emitBuffer[1] = publicCode[2];
    emitBuffer[2] = publicCode[1];
    emitBuffer[3] = publicCode[0];
    
    emitBuffer[4] = (uint8_t)random(256);
    emitBuffer[5] = frameCount;
    emitBuffer[6] = (uint8_t)((msg.length() > 250) ? 250 : msg.length());
    emitFrames((char*)msg.c_str(), msg.length());
  }

  void sendFriendKey(void){
    Serial.printf("Sharing key\n");
    resetRadioPacket();

    File pubKeyFile = SD.open(PUBLIC_KEY_LOC, FILE_READ);
    uint8_t frameCount = 0;
    size_t pubKeySize = pubKeyFile.size();
    pubKeyFile.read(fileData, pubKeySize);
    pubKeyFile.close();
    
    for(int i=0; i<pubKeySize; i++){
      frameCount++;
      i=250*frameCount;
    }

    for(int i=0; i<256; i++)
      emitBuffer[i] = 0;
    emitBuffer[0] = 0x00;
    emitBuffer[1] = keyshareCode[2];
    emitBuffer[2] = keyshareCode[1];
    emitBuffer[3] = keyshareCode[0];
    
    emitBuffer[4] = (uint8_t)random(256);
    emitBuffer[5] = frameCount;
    emitBuffer[6] = (uint8_t)((pubKeySize > 250) ? 250 : pubKeySize);
    emitFrames((char *)fileData, pubKeySize);
  }

  void recvMessage(void){
    if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
        digitalWrite(BOARD_SDCARD_CS, HIGH);
        digitalWrite(RADIO_CS_PIN, HIGH);
        digitalWrite(BOARD_TFT_CS, HIGH);
      if (listening) {
        if(rxFlag){
          enableInterrupt = false;

          int numBytes = radio.getPacketLength();
          int state = radio.readData(emitBuffer, numBytes);
          if (state == RADIOLIB_ERR_NONE && numBytes > 7 && numBytes <= 256) {
            int _code = parseCode(radioPacket[0].p_code);
            switch(_code){
              case __RECV_CODE_PUBLIC:
                newPacketReceived = true;
                processPublicMessage();
                break;
              case __RECV_CODE_KEYSHARE:
                processKeyshareMessage();
                break;
              case __RECV_CODE_ENCRYPTED:
                processEncryptedMessage();
                break;
            }
          }
          enableInterrupt = true;
          rxFlag = false;
          radio.startReceive();
        }
        
      }else{
        listening = true;
        radio.clearPacketSentAction();
        radio.setPacketReceivedAction(Setup_setRxFlag);
        radio.startReceive();
      }
      xSemaphoreGive( xSemaphore );
    }
  }

  void sendMessage(uint8_t *msg, size_t msgLen){
    if(msgLen <= 0)
      return;
    if (xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE) {
        digitalWrite(BOARD_SDCARD_CS, HIGH);
        digitalWrite(RADIO_CS_PIN, HIGH);
        digitalWrite(BOARD_TFT_CS, HIGH);
      transmissionFlag = true;
      if (hasRadio) {
        
        if (txFlag){
          transmissionFlag = true;
          radio.clearPacketReceivedAction();
          listening = false;
          radio.setPacketSentAction(Setup_setTxFlag);
          transmissionState = radio.startTransmit(msg, msgLen);
          if (!(transmissionState == RADIOLIB_ERR_NONE)){
              Serial.printf("failed to send %ld bytes, code ", msgLen);
              Serial.println(transmissionState);
          }
          while(transmissionFlag){
            delay(5);
          }
        }
      }
      xSemaphoreGive( xSemaphore );
    }
  }
}palcomRadio;