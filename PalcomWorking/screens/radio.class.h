#define __RECV_CODE_PUBLIC 0
#define __RECV_CODE_KEYSHARE 1
#define __RECV_CODE_ENCRYPTED 2
#define __RECV_CODE_OKAY 3

#define RADIO_PACKET_TIMEOUT 15

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
    		char keyshareCode[3] = {'\xfa', '\xaf', '\x32'};
    		char encryptedCode[3] = {'\xe9', '\xc7', '\x49'};
    		char okayCode[3] = {'\xa0','\xca', '\x71'};

    		uint8_t emitBuffer[256] = {0};
    		uint8_t recvBuffer[256] = {0};
    		char tmpName[128];
    		char shaHash[33];
    		palcom_packet_t *radioPacket;
    		palcom_packet_t *emitPacket;
    		palcom_packet_t *recvPacket;

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

    size_t appendToFile(const char *target, uint8_t * buf, size_t bufSize, bool addUser=false, bool appendNewline=false, bool external=false){
      File targetFd;
      size_t targetSize = 0;
      for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
        fileData[i] = 0;
      }
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
      if(external){
      	targetFd.printf("\n\t 1\n");
      }
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
        for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
          compBuffer[i] = 0;
        }
        File tmpFile = SD.open(tmpName, FILE_READ);
        tmpFile.read((uint8_t *)compBuffer, msgSize);
        tmpFile.close();
        SD.remove(tmpName);
      }

      for(int i=0; i<33; i++){
        shaHash[i] = compBuffer[i];
      }
      sprintf(tmpName, "%s/%s", pfs_dir_requests, shaHash);
      if(SD.exists(tmpName)){
        return;
      }
      for(int i=33; i<msgSize; i++){
        fileData[i-33] = compBuffer[i];
      }
      File hashFile = SD.open(tmpName, FILE_WRITE, O_TRUNC);
      hashFile.write(fileData, msgSize);
      hashFile.close();
      Serial.printf("Received and stored a public key : (%ld) %s\n", msgSize, shaHash);
      newPacketReceived = true;
    }

    void processEncryptedMessage(void){
      if(radioPacket == NULL){
        Serial.printf("Null Packet.\n");
        return;
      }
      if(!SD.exists(pfs_dir_tmp)){
        SD.mkdir(pfs_dir_tmp);
      }
      PalcomFS pfs;
      
      // Handle Multi framed packet
      size_t msgSize = 0;
      Serial.printf("Frame Count : %d\n", radioPacket[0].p_count);
      sprintf(tmpName, "%s/enc.%d", pfs_dir_tmp, radioPacket[0].p_id);
      if(radioPacket[0].p_count > 1){
        appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        return;
      }else if(SD.exists(tmpName)){
        msgSize = appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
          fileData[i] = 0;
        File tmpFile = SD.open(tmpName, FILE_READ);
	msgSize = tmpFile.size();
        tmpFile.read((uint8_t *)fileData, msgSize);
        tmpFile.close();
        SD.remove(tmpName);
      }
//	int err = doneProcessing();
//      	if(err <= -1)
//        	return;
//      size_t msgSize = (size_t)err;

      Serial.printf("C : File size : %ld\n", msgSize);
      for(int i=0; i<33; i++)
        shaHash[i] = fileData[i];
      Serial.printf("Recevied Friend Hash : %s\n", shaHash);

      for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
	      compBuffer[i];

      for(int i=33; i<msgSize; i++){
        compBuffer[i-33] = fileData[i];
      } 

      sprintf(tmpName, "/friends/%s", shaHash);
      if(!SD.exists(tmpName)){
        Serial.printf("Friend folder doesn't exist : %s\n", tmpName);
        return;
      }

      	sprintf(tmpName, "%s/msgLog", tmpName);
	Serial.printf("Received decrypting  message of size (%ld) to '%s'.\n", msgSize, pfs_file_cryptRecv);
      	if(!rsaDecrypt((const unsigned char *)compBuffer, msgSize-33, pfs_file_cryptRecv)){
      		Serial.printf("Decryption failed.\n");
        	return;
      	}
      
      	File res = SD.open(pfs_file_cryptRecv, FILE_READ);
      	res.read((uint8_t *)compBuffer, res.size());
      	msgSize = res.size();
      	res.close();

      // Write packet to msgLog
      if(!SD.exists(tmpName)){
        if(msgSize > 0) // Multi frame packet
          appendToFile(tmpName, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(tmpName, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }else{
        File msgFile = SD.open(tmpName, FILE_READ);
        size_t msgFileSize = msgFile.size();
        msgFile.close();
        if(msgFileSize > 100000)
          SD.remove(tmpName);

        if(msgSize > 0)
          appendToFile(tmpName, (uint8_t *)compBuffer, msgSize, false, false);
        else
          appendToFile(tmpName, radioPacket[0].p_content, radioPacket[0].p_size, true, true);
      }
    }

    int doneProcessing(){
    	size_t msgSize = 0;
      	sprintf(tmpName, "%s/tmp.%d", pfs_dir_tmp, radioPacket[0].p_id);
      	if(radioPacket[0].p_count > 1){
        	appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        	return -1;
      	}else if(SD.exists(tmpName)){
        	msgSize = appendToFile((const char *)tmpName, radioPacket[0].p_content, radioPacket[0].p_size, false, false);
        	File tmpFile = SD.open(tmpName, FILE_READ);
        	tmpFile.read((uint8_t *)compBuffer, msgSize);
        	tmpFile.close();
        	SD.remove(tmpName);
		compBuffer[msgSize] = '\n';msgSize++;
		compBuffer[msgSize] = '\t';msgSize++;
		compBuffer[msgSize] = ' ';msgSize++;
		compBuffer[msgSize] = '1';msgSize++;
		compBuffer[msgSize] = '\n';msgSize++;
		return msgSize;
      	}
	sprintf(compBuffer, "%s\n\t 1\n", radioPacket[0].p_content);
	msgSize = 5 + radioPacket[0].p_size;
	return msgSize;
    }

	void processPublicMessage(void){
      		if(radioPacket == NULL){
        		return;
      		}
      		if(!SD.exists(pfs_dir_public)){
      	  		SD.mkdir(pfs_dir_public);
      		}
      		if(!SD.exists(pfs_folder_recvQueue)){
	      		SD.mkdir(pfs_folder_recvQueue);
	      	}
      		if(!SD.exists(pfs_folder_sendQueue)){
	      		SD.mkdir(pfs_folder_sendQueue);
	      	}

		sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
	      	sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
	      	// check for repeate message
	      	if(!SD.exists(compBuffer) && !SD.exists(tmpName) && recvPacket->p_count <= 1){
			Serial.printf("Processing single frame message.\n");
			appendToFile(pfs_file_publicLog, recvPacket[0].p_content, recvPacket[0].p_size, false, false, true);
                        fileData[4] = recvPacket->p_id;
                        fileData[5] = recvPacket->p_count;
                        fileData[6] = 1;
			fileData[7] = 'O';
      			sendQueueAdd((char *)fileData, 8, okayCode_int);
			newPacketReceived = true;
			return;
      		}
		
		sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
		if(!SD.exists(tmpName) && recvPacket->p_count > 1){
			Serial.printf("First multi packet receipt.\n");
			File f = SD.open(tmpName, FILE_WRITE);
			f.printf("%c\n", recvPacket->p_count);
                        appendToFile(tmpName, recvPacket[0].p_content, recvPacket[0].p_size, false, false, false);
			fileData[4] = recvPacket->p_id;
                        fileData[5] = recvPacket->p_count;
                        fileData[6] = 1;
                        fileData[7] = 'O'; 
                        sendQueueAdd((char *)fileData, 8, okayCode_int);
			return;
		}else if(SD.exists(tmpName)){
			PalcomFS pfs; pfs.clearFileBuffer();
			File f = SD.open(tmpName, FILE_READ);
			size_t s = (f.size() < 2) ? f.size() : 2;
			f.read(fileData, s);
			f.close();
			Serial.printf("Continuing the processing of a multiframed message. %d == %d\n", fileData[0], recvPacket->p_count);
			if(fileData[0] == recvPacket->p_count)
				return;
			appendToFile(tmpName, recvPacket[0].p_content, recvPacket[0].p_size, false, false, false);
			fileData[4] = recvPacket->p_id;
                        fileData[5] = recvPacket->p_count;
                        fileData[6] = 1;
                        fileData[7] = 'O';
                        sendQueueAdd((char *)fileData, 8, okayCode_int);
		}

		if(recvPacket->p_count <= 1 && SD.exists(tmpName)){
			Serial.printf("Adding received multipacket message to message log.\n");
			File f = SD.open(tmpName, FILE_READ);
			size_t s = f.size() - 2;
			f.seek(2);
			PalcomFS pfs; pfs.clearFileBuffer();
			f.read(fileData, s);
			f.close();
			appendToFile(pfs_file_publicLog, fileData, s, false, false, true);
			pfs.rm(tmpName);
			newPacketReceived = true;
		}
		return;

      // Handle Multi framed packet
      int err = doneProcessing();
      if(err <= -1)
	      return;
      size_t msgSize = (size_t)err;
      
	Serial.printf("Storing received message (%ld): \n", msgSize);
      	for(int i=0; i<msgSize; i++){
		Serial.printf("%c", compBuffer[i]);
	}Serial.printf("\n");

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
      if(code == encryptedCode_int){
        return __RECV_CODE_ENCRYPTED;
      }
      if(code == okayCode_int){
      	return __RECV_CODE_OKAY;
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
	    Serial.printf("Sending Message Frame.\n");
            sendMessage(emitBuffer, emitSize);
            delay(200);
            emitSize = 7;
            emitBuffer[5]--;
            sendRequired = false;
          }
        }

        if(sendRequired){
          emitBuffer[6] = emitSize-7;
	    Serial.printf("Sending Final Message Frame.\n");
          sendMessage(emitBuffer, emitSize);
        }
      }
    }

    		void toggleSendMode(){
			transmissionFlag = true;
                        radio.clearPacketReceivedAction();
                        listening = false;
               		radio.setPacketSentAction(Setup_setTxFlag);
		}

		void toggleRecvMode(){
			listening = true;
			radio.clearPacketSentAction();
                        radio.setPacketReceivedAction(Setup_setRxFlag);
                	radio.startReceive();	
		}
	public:
   		uint32_t publicCode_int = 0x33445500;
    		uint32_t keyshareCode_int = 0xfaaf3200;
    		uint32_t encryptedCode_int = 0xe9c74900;
		uint32_t okayCode_int = 0xa0ca7100;


  		PalcomRadio(){
    			radioPacket = (palcom_packet_t*)malloc(sizeof(palcom_packet_t));
			emitPacket = (palcom_packet_t*)malloc(sizeof(palcom_packet_t));
    			recvPacket = (palcom_packet_t*)malloc(sizeof(palcom_packet_t));
    			radioPacket = (palcom_packet_t*)emitBuffer;
    			emitPacket = (palcom_packet_t*)emitBuffer;
    			recvPacket = (palcom_packet_t*)recvBuffer;
  		}
  
		void sendEncryptedMessage(const uint8_t *msg, size_t msgSize){
			Serial.printf("Sending Message of size %ld\n", msgSize);
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
	    		Serial.printf("Encrypted Send : Frame count %d\n", emitBuffer[5]);
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
    resetRadioPacket();
    PalcomFS pfs;
    pfs.getPublicHash(shaHash);

    File pubKeyFile = SD.open(pfs_file_keysPublic, FILE_READ);
    uint8_t frameCount = 0;
    size_t pubKeySize = pubKeyFile.size();
    pubKeyFile.read((uint8_t *)compBuffer, pubKeySize);
    pubKeyFile.close();

    pubKeySize+=33;
    for(int i=0; i<33; i++){
      fileData[i] = shaHash[i];
    }
    for(int i=0; i<pubKeySize; i++){
      fileData[i+33] = compBuffer[i];
    }
    
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


  		bool processOkayMessage(void){
			Serial.printf("Processing OKAY message.\n");
			// using radio packet, check if there's a file using this packet's id.
			PalcomFS pfs;
			sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
			if(!SD.exists(fileNameBuffer)){
				Serial.printf("\t~%s doesn't exist.\n", fileNameBuffer);
				return false;
			}
			Serial.printf("\tReceived a valid OKAY\n");

			// open file and read the first 7 bytes.
			File fd = SD.open(fileNameBuffer, FILE_READ);
			pfs.clearFileBuffer();
			size_t fileSize = fd.size();
			fd.read(fileData, fileSize);
			fd.close();
			palcom_packet_t *grabber;
		       	grabber = (palcom_packet_t *)&fileData;
			
			Serial.printf("\tCurrent file size: %ld\n", fileSize);
			if(grabber->p_count != recvPacket->p_count ){
				Serial.printf("\t~Compairison : %d vs %d\n", grabber->p_count, recvPacket->p_count);
				return false;
			}
			Serial.printf("\tOkayed message needs to be processed\n");
			
			// update packet header, remove chunk x from file.
			int newSize = fileSize-7-250;
			if(newSize <= 0){
				Serial.printf("\t~Deleting okay message? (%d)\n", newSize);
				pfs.rm(fileNameBuffer);
				return false;
			}
			Serial.printf("\tOkayed message new size : %ld\n", newSize);
			int offset = 257;
			for(int i=offset; i<fileSize; i++){
				compBuffer[i-offset] = fileData[i];
			}

			for(int i=7; i<fileSize; i++){
				if(i<newSize)
					fileData[i] = compBuffer[i];
				else
					fileData[i] = 0;
			}
			grabber->p_count--;
			if(fileSize > 257){
				grabber->p_size = 250;
			}else{
				grabber->p_size = fileSize-7;
			}
			
			fd = SD.open(fileNameBuffer, FILE_WRITE, O_TRUNC);
			fd.write(fileData, fileSize-250);
			fd.close();
			Serial.printf("\tOkayed message successful?\n");
			timeout = RADIO_PACKET_TIMEOUT;
			return true;
		}
		
		bool recvMessage(void){
			bool ret = false;
		    	if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
		        	digitalWrite(BOARD_SDCARD_CS, HIGH);
		        	digitalWrite(RADIO_CS_PIN, HIGH);
		        	digitalWrite(BOARD_TFT_CS, HIGH);
		      		if (listening) {
					if(rxFlag){
				          	enableInterrupt = false;
				
		        		  	int numBytes = radio.getPacketLength();
		        		  	int state = radio.readData(recvBuffer, numBytes);
						Serial.printf("(%x) ID: %d, Received %d bytes, %d frames remaining\n", recvPacket->p_code, recvPacket->p_id, numBytes, recvPacket->p_count);
						if (state == RADIOLIB_ERR_NONE && numBytes > 7 && numBytes <= 256) {
		        		    		int _code = parseCode(recvPacket->p_code);
		        		    		switch(_code){
		        		      			case __RECV_CODE_PUBLIC:
									Serial.printf("Received public message (%ld)\n", numBytes);
		        		        			processPublicMessage();
		        		        			break;
		        		      			case __RECV_CODE_KEYSHARE:
		        		        			processKeyshareMessage();
		        		        			break;
		        		      			case __RECV_CODE_ENCRYPTED:
		        		        			processEncryptedMessage();
		        		        			break;
								case __RECV_CODE_OKAY:
									Serial.printf("Received Okay message\n");
									ret = processOkayMessage();
									break;
					              		default:
					                		Serial.printf("invalid code\n");
					            	}
						}
		        		  	enableInterrupt = true;
		        		  	rxFlag = false;
		        		  	radio.startReceive();
		        		}
		        	
				}else{
					toggleRecvMode();
		      		}
		      		xSemaphoreGive( xSemaphore );
		    	}
			return ret;
		}

  		void sendMessage(uint8_t *msg, size_t msgLen){
    			if(msgLen <= 0)
      				return;
    			if (xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE) {
        			digitalWrite(BOARD_SDCARD_CS, HIGH);
        			digitalWrite(RADIO_CS_PIN, HIGH);
        			digitalWrite(BOARD_TFT_CS, HIGH);
      				transmissionFlag = true;
      				if (hasRadio && txFlag) {
					toggleSendMode();
        	  			transmissionState = radio.startTransmit(msg, msgLen);
        	  			if (!(transmissionState == RADIOLIB_ERR_NONE)){
        	      				Serial.printf("failed to send %ld bytes, code ", msgLen);
        	      				Serial.println(transmissionState);
						transmissionFlag = false;
        	  			}
        	  			while(transmissionFlag){
        	    				delay(5);
       	   				}
        			}
      				xSemaphoreGive( xSemaphore );
    			}
  		}

  	void appendGeneralMessage(string msg){
	  	if(!SD.exists(pfs_dir_public)){
        		SD.mkdir(pfs_dir_public);
		}
	  	if(msg.length() > 0){
 			appendToFile(pfs_file_publicLog, (uint8_t *)msg.c_str(), msg.length(), false, false); 
			newPacketReceived = true;
	  	}
  	}


	uint8_t getFrameCount(size_t size){
		uint8_t frameCount = 0;
    		for(int i=0; i<size; i++){
      			frameCount++;
      			i=250*frameCount;
    		}
		return frameCount;
	}
	bool sendQueueAdd(char *buf, size_t bufSize, uint32_t code){
		if(buf == NULL || bufSize <= 0){
			return false;
		}

		if(!SD.exists(pfs_folder_sendQueue)){
			SD.mkdir(pfs_folder_sendQueue);
		}
		
		PalcomFS pfs;
  		uint8_t p_id = (uint8_t)random(256);
		uint8_t frameCount = this->getFrameCount(bufSize);
  		uint8_t p_size = bufSize <= 250 ? bufSize : 250;
		switch(code){
			//publicCode 
			case 0x33445500:
				sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, p_id);
				Serial.printf("(public) Message Queued to '%s'\n", compBuffer);
				pfs.fd = SD.open(compBuffer, FILE_WRITE, O_TRUNC);
				pfs.clearFileBuffer();
				fileData[0] = 0x00;
    				fileData[1] = publicCode[2];
    				fileData[2] = publicCode[1];
    				fileData[3] = publicCode[0];

				fileData[4] = p_id;
				fileData[5] = frameCount;
				fileData[6] = p_size;
				pfs.fd.write(fileData, 7);

				pfs.addToFiledata(buf, bufSize);
				pfs.fd.write(fileData, bufSize);
				pfs.fd.close();
				Serial.printf("Wrote message to send queue folder.\n");
				break;
                	//keyshareCode
		       	case 0xfaaf3200:
				break;
                	//encryptedCode
			case 0xe9c74900:
				break;
			//okayCode
			case 0xa0ca7100:
				sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, fileData[4]);
				Serial.printf("(okay) Message Queued to '%s'\n", compBuffer);
				pfs.fd = SD.open(compBuffer, FILE_WRITE, O_TRUNC);
                                fileData[0] = 0x00;
                                fileData[1] = okayCode[2];
                                fileData[2] = okayCode[1];
                                fileData[3] = okayCode[0];

                                /*fileData[4] = p_id;
                                fileData[5] = frameCount;
                                fileData[6] = p_size;
                                pfs.fd.write(fileData, 7);

                                pfs.addToFiledata(buf, bufSize);*/
                                pfs.fd.write(fileData, bufSize);
                                pfs.fd.close();
				break;
			default:
				return false;
		}

		//pfs.fd = SD.open();
		return true;
	}
	int previousId = -1;
	int previousFrameCount = -1;
	int timeout = RADIO_PACKET_TIMEOUT;
	void sendQueue(void){
		lv_task_handler();
		PalcomFS pfs;
		if(!pfs.popSendQueue()){
			return;
		}
		Serial.printf("sending contents of '%s'\n", fileNameBuffer);
		toggleSendMode();
		lv_task_handler();

    		//pfs.fd = SD.open((const char *)fileNameBuffer, FILE_READ);
		lv_task_handler();
		this->resetRadioPacket();
		pfs.clearFileBuffer();
		lv_task_handler();

		size_t fileSize = pfs.fd.size();
		if(fileSize > 255)
			fileSize = 255;
		pfs.fd.read(fileData, fileSize);
		pfs.fd.close();
		lv_task_handler();

		for(int i=0; i<256; i++){
			emitBuffer[i] = fileData[i];
		}
		if(previousId != radioPacket->p_id){
			previousId = radioPacket->p_id;
			previousFrameCount = radioPacket->p_count;
			timeout = RADIO_PACKET_TIMEOUT;
		}else if(previousId == radioPacket->p_id && previousFrameCount != radioPacket->p_count){
			timeout = RADIO_PACKET_TIMEOUT;
			previousFrameCount = radioPacket->p_count;
		}

		Serial.printf("%d ID : %d, %d chunks remaining, timeout(%d)\n", radioPacket->p_id, fileSize, radioPacket->p_count, timeout);
		sendMessage(emitBuffer, fileSize);
		if(timeout <= 0 /*|| radioPacket->p_code == okayCode_int*/){
			pfs.rm((const char *)fileNameBuffer);
			timeout = -1;
			previousId = -1;
		}else if(timeout > 0){
			timeout--;
		}

		toggleRecvMode();
	}

	void recvQueue(){
		
	}
}palcomRadio;
