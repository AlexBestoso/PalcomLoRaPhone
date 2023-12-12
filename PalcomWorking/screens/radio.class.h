#define __RECV_CODE_PUBLIC 0
#define __RECV_CODE_KEYSHARE 1
#define __RECV_CODE_ENCRYPTED 2
#define __RECV_CODE_OKAY 3
#define __RECV_CODE_CONFIRM 4
#define __RECV_CODE_DONE 5

#define RADIO_PACKET_TIMEOUT 10

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
		char confirmCode[3] = {'\xaa', '\xbd', '\xee'};
		char doneCode[3] = {'\xd0', '\x43', '\x90'};

    		uint8_t emitBuffer[256] = {0};
    		uint8_t recvBuffer[256] = {0};
    		char tmpName[128];
    		char shaHash[33];
    		palcom_packet_t *radioPacket;
    		palcom_packet_t *emitPacket;
    		palcom_packet_t *recvPacket;

		/*
		 * Tool / Useful functions.
		 * */
    		void resetRadioPacket(){
      			if(radioPacket == NULL)
        			return;
      			for(int i=0; i<256; i++){
      				emitBuffer[i] = 0;
				recvBuffer[i] = 0;
      			}
    		}

		void decrememntQueuedPacket(){
			PalcomFS pfs;
			if(!pfs.popSendQueue())
				return;
			
			pfs.fd.close();
		}

		/*
		 *  Message Sender Functions.
		 * */
  		int processOkayMessage(void){
			Serial.printf("Processing Okay Message.\n");
			// using radio packet, check if there's a file using this packet's id.
			PalcomFS pfs;
			sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
			if(!SD.exists(fileNameBuffer)){
				return 2; // respond with done message.
			}

			File fd = SD.open(fileNameBuffer, FILE_READ);
			pfs.clearFileBuffer();
			size_t fileSize = fd.size();
			fd.read(fileData, fileSize);
			fd.close();

			palcom_packet_t *grabber = (palcom_packet_t *)&fileData;
			
			// Ensure that the okay message is for the packet that we just sent and that we don't double process.
			if(grabber->p_count != recvPacket->p_count ){
				// We may be able to send another confirm message here to ensure the okay messages stop in a timely fassion.
				if((grabber->p_count+1) == recvPacket->p_count){
					return 1; // respond with confirm message.
				}
				return 0; // neither done nor confirm
			}
			
			// new Size of the content section.
			// 7 = header size
			// 250 = size of content buffer
			int newSize = fileSize-250;

			// if content size is 0, there's no more data to send.
			if(newSize-7 <= 0){
				sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
                        	pfs.rm(fileNameBuffer);
				return 2; // respond with done message.
			}

			pfs.clearCompBuffer();
			size_t compBufferSize = 7;
			
			grabber->p_count--; // decrement the frame count
			// Update the size of the content section
			if(newSize-7 > 250)
                                grabber->p_size = 250;
                        else
                                grabber->p_size = newSize-7;

			// add the updated message header to the buffer
			for(int i=0; i<7; i++)
				compBuffer[i] = fileData[i];
			// Add the remaining data to the buffer
			for(int i=255; i<fileSize; i++){
				compBuffer[i-(255-7)] = fileData[i];
				compBufferSize++;
			}

			// OverWrite the sendqueue file.
			fd = SD.open(fileNameBuffer, FILE_WRITE, O_TRUNC);
			fd.write((uint8_t *)compBuffer, compBufferSize);
			fd.close();
			timeout = RADIO_PACKET_TIMEOUT;

			return 1; // respond with confirm message.
		}

		void sendConfirmMessage(){
			Serial.printf("Sending Confirm Message.\n");
			PalcomFS pfs;
			pfs.clearFileBuffer();
			toggleSendMode();
			fileData[4] = recvPacket->p_id;
			fileData[5] = recvPacket->p_count;
			fileData[6] = 1;
			fileData[7] = recvPacket->p_content[0];
			sendQueueAdd((char *)fileData, 8, confirmCode_int);
			toggleRecvMode();
		}

		void sendDoneMessage(){
			Serial.printf("Sending done message.\n");
			PalcomFS pfs;
			pfs.clearFileBuffer();
			toggleSendMode();
			fileData[4] = recvPacket->p_id;
			fileData[5] = recvPacket->p_count;
			fileData[6] = 1;
			fileData[7] = recvPacket->p_content[0];
			sendQueueAdd((char *)fileData, 8, doneCode_int);
			toggleRecvMode();
		}

		/*
		 * Message Receiver Functions.
		 * */
		bool processDoneMessage(void){
			PalcomFS pfs;
			Serial.printf("Processing done message.\n");
			sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
			if(SD.exists(tmpName)){
                                pfs.clearFileBuffer();
                                File f = SD.open(tmpName, FILE_READ);
                                size_t s = f.size();
                                f.read(fileData, s);
                                f.close();
				if(recvPacket->p_content[0] == 'K'){
					Serial.printf("Finalizing keyshare.\n");
					finalizeKeyShare(fileData, s);
					sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
				}else if(recvPacket->p_content[0] == 'E'){
					Serial.printf("Finalizing encrypted message.\n");
					finalizeEncryptedMessage(fileData, s);
					sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
				}else{
					Serial.printf("Finalizing public message.\n");
	                                appendGeneralMessage((const char *)fileData, true);
				}
                                newPacketReceived = true;
                        }
			sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
			pfs.rm(compBuffer);
			pfs.rm(tmpName);
			return true;
		}
    	
		bool processConfirmMessage(void){
			PalcomFS pfs;
			Serial.printf("Processing confirm message.\n");
			sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id);
			pfs.rm(compBuffer);
			return true;
		}
		
		bool processPublicMessage(void){
      			if(radioPacket == NULL){
        			return false;
      			}
			PalcomFS pfs;
      			if(!SD.exists(pfs_dir_public)){
      	  			SD.mkdir(pfs_dir_public);
      			}
      			if(!SD.exists(pfs_folder_recvQueue)){
	      			SD.mkdir(pfs_folder_recvQueue);
	      		}
      			if(!SD.exists(pfs_folder_sendQueue)){
	      			SD.mkdir(pfs_folder_sendQueue);
	      		}

	      		sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
			if(!SD.exists(tmpName)){
                	        pfs.fileAppend(tmpName, recvPacket[0].p_content, recvPacket[0].p_size);
				pfs.clearFileBuffer();
				fileData[4] = recvPacket->p_id;
                	        fileData[5] = recvPacket->p_count;
                	        fileData[6] = 1;
                	        fileData[7] = 'O'; 
                	        sendQueueAdd((char *)fileData, 8, okayCode_int);
				if(recvPacket->p_count > 1)
					return true;
			}else if(SD.exists(tmpName)){
				// Check to ensure we're not double parsing a packet and if there's a count bumping attack
				// Also check for packet decrement attack.
				// FileData minus 1 should equal the received packet count in order to contnue.
				sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id); // checks if okay was already sent.
				if(SD.exists(fileNameBuffer)){
					return false;
				}
				pfs.fileAppend(tmpName, recvPacket[0].p_content, recvPacket[0].p_size);
				pfs.clearFileBuffer();
				fileData[4] = recvPacket->p_id;
                	        fileData[5] = recvPacket->p_count;
                	        fileData[6] = 1;
                	        fileData[7] = 'O';
                	        sendQueueAdd((char *)fileData, 8, okayCode_int);
			}
			return true;
    		}

    		void processKeyshareMessage(){
			Serial.printf("Processing keyshare message.\n");
      			if(radioPacket == NULL){
        			return;
      			}

			PalcomFS pfs;

			/* New code start*/
			if(!SD.exists(pfs_dir_public)){
                        	SD.mkdir(pfs_dir_public);
                	}
                	if(!SD.exists(pfs_folder_recvQueue)){
                	        SD.mkdir(pfs_folder_recvQueue);
               	 	}
                	if(!SD.exists(pfs_folder_sendQueue)){
                	        SD.mkdir(pfs_folder_sendQueue);
                	}

                        sprintf(tmpName, "%s/%d.queued", pfs_folder_recvQueue, recvPacket->p_id);
			int errorCorrect = 0;
                        if(!SD.exists(tmpName)){
				Serial.printf("Processing first chunk of keyshare. (%ld)\n", (long)recvPacket->p_size);
				if(recvPacket->p_size >= 250)
					errorCorrect = 2;
				else
					errorCorrect = -2;
                                pfs.fileAppend(tmpName, recvPacket->p_content, recvPacket->p_size-errorCorrect);
                                pfs.clearFileBuffer();
                                fileData[4] = recvPacket->p_id;
                                fileData[5] = recvPacket->p_count;
                                fileData[6] = 1;
                                fileData[7] = 'K';
				Serial.printf("Sending okay message.\n");
                                sendQueueAdd((char *)fileData, 8, okayCode_int);
                                if(recvPacket->p_count > 1)
                                        return;// true;
                        }else if(SD.exists(tmpName)){
				Serial.printf("Receiving remaining keyshare files. (%ld)\n", (long)recvPacket->p_size);
				if(recvPacket->p_size >= 250)
                                        errorCorrect = 2;
                                else
                                        errorCorrect = -2;
                                // Check to ensure we're not double parsing a packet and if there's a count bumping attack
                                // Also check for packet decrement attack.
                                // FileData minus 1 should equal the received packet count in order to contnue.
                                sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id); // checks if okay was already sent.
                                if(SD.exists(fileNameBuffer)){
                                        return;// false;
                                }
                                pfs.fileAppend(tmpName, recvPacket[0].p_content, recvPacket[0].p_size-errorCorrect);
                                pfs.clearFileBuffer();
                                fileData[4] = recvPacket->p_id;
                                fileData[5] = recvPacket->p_count;
                                fileData[6] = 1;
                                fileData[7] = 'K';
				Serial.printf("Sending okay message.\n");
                                sendQueueAdd((char *)fileData, 8, okayCode_int);
                        }
    		}

		void finalizeKeyShare(unsigned char *buf, size_t size){
			Serial.printf("Finalizing keyshare, %ld bytes\n", size);
			PalcomFS pfs;
			// Error handling.
			if(size <= 33){
				Serial.printf("Invalid key size.\n");
				return;
			}
			if(size-33 > 1032){
				Serial.printf("Correcting key size.\n");
				size= 1032+33;
			}

			// Extract sha hash.
			for(int i=0; i<33; i++){
                                shaHash[i] = buf[i];
                        }
                        sprintf(tmpName, "%s/%s", pfs_dir_requests, shaHash);
                        if(SD.exists(tmpName)){
				Serial.printf("Key request already exists.\n");
                                return;
                        }
			pfs.clearCompBuffer();
			// Extract key data.
			for(int i=33; i<size; i++){
                                compBuffer[i-33] = buf[i];
                        }
                        File hashFile = SD.open(tmpName, FILE_WRITE, O_TRUNC);
                        hashFile.write((const uint8_t *)compBuffer, size-33);
                        hashFile.close();
                        Serial.printf("Received and stored a public key to %s: (%ld) %s\n", tmpName, size-33, shaHash);
		}

		/*
		 * Other Functions
		 * */

	    size_t appendToFile(const char *target, uint8_t * buf, size_t bufSize, bool addUser=false, bool appendNewline=false, bool external=false){
		    	PalcomFS pfs;
		      	File targetFd;
		      	size_t targetSize = 0;

			pfs.clearFileBuffer();
		      	
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

		void finalizeEncryptedMessage(unsigned char *message, size_t messageSize){
			if(messageSize <33){
				Serial.printf("Invalid encrypted message.\n");
				return;
			}
			for(int i=0; i<33; i++)
                                shaHash[i] = message[i];
                        Serial.printf("Recevied Friend Hash : %s\n", shaHash);

			PalcomFS pfs;
			pfs.clearCompBuffer();

                        for(int i=33; i<messageSize; i++){
                                compBuffer[i-33] = message[i];
                        }

                        sprintf(tmpName, "/friends/%s", shaHash);
                        if(!SD.exists(tmpName)){
                                Serial.printf("Friend folder doesn't exist : %s\n", tmpName);
                                return;
                        }

			sprintf(tmpName, "%s/msgLog", tmpName);
                        Serial.printf("Received decrypting  message of size (%ld) to '%s'.\n", messageSize, pfs_file_cryptRecv);
			if(SD.exists(pfs_file_cryptRecv))
				SD.remove(pfs_file_cryptRecv);
                        if(!rsaDecrypt((const unsigned char *)compBuffer, messageSize-33, pfs_file_cryptRecv)){
                                Serial.printf("Decryption failed.\n");
                                return;
                        }

			pfs.clearCompBuffer();
                        File res = SD.open(pfs_file_cryptRecv, FILE_READ);
                        res.read((uint8_t *)compBuffer, res.size());
                        messageSize = res.size();
                        res.close();
			SD.remove(pfs_file_cryptRecv);

			// Write packet to msgLog
			Serial.printf("Populating message log with the text : %s\n", (const char *)compBuffer);
                        if(!SD.exists(tmpName)){
                                        appendToFile(tmpName, (uint8_t *)compBuffer, messageSize, false, false);
                        }else{
                                File msgFile = SD.open(tmpName, FILE_READ);
                                size_t msgFileSize = msgFile.size();
                                msgFile.close();
                                if(msgFileSize > 100000)
                                        SD.remove(tmpName);

                                appendToFile(tmpName, (uint8_t *)compBuffer, messageSize, false, false);
                        }
		}

    		void processEncryptedMessage(void){
			Serial.printf("Processing encrypted message.\n");
                        if(radioPacket == NULL){
                                return;
                        }

                        /* New code start*/
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
			PalcomFS pfs;
                        if(!SD.exists(tmpName)){
                                Serial.printf("Checking if encrypted message is directed to me.\n");
				pfs.clearCompBuffer();
				for(int i=0; i<33; i++)
					compBuffer[i] = recvPacket->p_content[i];
				if(!pfs.validFriend((const char *)compBuffer)){
					Serial.printf("Friend is not in list.\n");
					return;
				}

				Serial.printf("Valid friend detected.\n");
				pfs.clearFileBuffer();
                                File f = SD.open(tmpName, FILE_WRITE);
                                f.printf("%c\n", recvPacket->p_count);
                                f.close();
                                appendToFile(tmpName, recvPacket[0].p_content, recvPacket[0].p_size, false, false, false);
                                fileData[4] = recvPacket->p_id;
                                fileData[5] = recvPacket->p_count;
                                fileData[6] = 1;
                                fileData[7] = 'E';
                                Serial.printf("Sending okay message.\n");
                                sendQueueAdd((char *)fileData, 8, okayCode_int);
                                if(recvPacket->p_count > 1)
                                        return;// true;
                        }else if(SD.exists(tmpName)){
				Serial.printf("Receiving remaining encrypted frames.\n");
                                pfs.clearFileBuffer();
                                File f = SD.open(tmpName, FILE_READ);
                                size_t s = f.size();
                                f.read(fileData, s);
                                f.close();
                                // Check to ensure we're not double parsing a packet and if there's a count bumping attack
                                // Also check for packet decrement attack.
                                // FileData minus 1 should equal the received packet count in order to contnue.
                                sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, recvPacket->p_id); // checks if okay was already sent.
                                if(SD.exists(compBuffer)){
                                        return;// false;
                                }
                                appendToFile(tmpName, recvPacket[0].p_content, recvPacket[0].p_size, false, false, false);
                                fileData[4] = recvPacket->p_id;
                                fileData[5] = recvPacket->p_count;
                                fileData[6] = 1;
                                fileData[7] = 'E';
                                Serial.printf("Sending okay message.\n");
                                sendQueueAdd((char *)fileData, 8, okayCode_int);
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
		if(code == confirmCode_int){
			return __RECV_CODE_CONFIRM;
		}
		if(code == doneCode_int){
			return __RECV_CODE_DONE;
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
		uint32_t confirmCode_int = 0xaabdee00;
                uint32_t doneCode_int = 0xd0439000;

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
    			PalcomFS pfs;
			pfs.clearCompBuffer();
    			pfs.getPublicHash(shaHash);

    			File pubKeyFile = SD.open(pfs_file_keysPublic, FILE_READ);
    			size_t pubKeySize = pubKeyFile.size();
    			pubKeyFile.read((uint8_t *)compBuffer, pubKeySize);
    			pubKeyFile.close();
	
			string msg = "";
			pfs.clearFileBuffer();
    			for(int i=0; i<33; i++){
      				fileData[i] = shaHash[i];
    			}
    			for(int i=0; i<pubKeySize; i++){
      				fileData[i+33] = compBuffer[i];
    			}
    			pubKeySize+=33;
	
			Serial.printf("Quing key of size %ld\n", pubKeySize);
			sendQueueAdd((char *)fileData, pubKeySize, keyshareCode_int);
  		}


		
		bool recvMessage(void){
			bool ret = false;
			int okay = 0;
			int sendConfirmation = false;
		    	if ( xSemaphoreTake( xSemaphore, portMAX_DELAY ) == pdTRUE ) {
				this->resetRadioPacket();
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
		        		        			processPublicMessage();
		        		        			break;
		        		      			case __RECV_CODE_KEYSHARE:
									Serial.printf("Received keyshare message(%ld)\n", numBytes);
									Serial.printf("Debug:\n\t");
									for(int i=0; i<numBytes; i++){
										Serial.printf("%c ", recvBuffer[i]);
									}Serial.printf("\n");
		        		        			processKeyshareMessage();
		        		        			break;
		        		      			case __RECV_CODE_ENCRYPTED:
									Serial.printf("Received encrypted message (%ld).\n", numBytes);
		        		        			processEncryptedMessage();
		        		        			break;
								case __RECV_CODE_OKAY:
									okay = processOkayMessage(); 
									sendConfirmation = okay;
									ret = (okay == 0) ? false : true;
									break;
								case __RECV_CODE_CONFIRM:
									ret = processConfirmMessage();
									break;
								case __RECV_CODE_DONE:
									ret = processDoneMessage();
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

			if(sendConfirmation == 1){
				sendConfirmMessage();
			}else if(okay == 2){
				sendDoneMessage();
			}
			this->resetRadioPacket();
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
				//txFlag = false;
      				xSemaphoreGive( xSemaphore );
    			}
  		}

  	void appendGeneralMessage(string msg, bool external=false){
		PalcomFS pfs;
	  	if(!SD.exists(pfs_dir_public)){
        		SD.mkdir(pfs_dir_public);
		}
		size_t msgSize = msg.length();
		pfs.clearCompBuffer();
		if(!external){
			compBuffer[0] = MESSAGE_LOCAL_START;
			for(int i=0; i<msgSize; i++){
				if(i+1 >= __GLOBAL_BUFFER_SIZE){
					break;
				}
				compBuffer[i+1] = msg[i];
			}
			int pos = (msgSize + 2 >= __GLOBAL_BUFFER_SIZE) ? __GLOBAL_BUFFER_SIZE-1 : msgSize+1;
			compBuffer[pos] = MESSAGE_LOCAL_END;
		}else{
			compBuffer[0] = MESSAGE_REMOTE_START;
                        for(int i=0; i<msgSize; i++){
                                if(i+1 >= __GLOBAL_BUFFER_SIZE){
                                        break;
                                }
                                compBuffer[i+1] = msg[i];
                        }
                        int pos = (msgSize + 2 >= __GLOBAL_BUFFER_SIZE) ? __GLOBAL_BUFFER_SIZE-1 : msgSize+1;
                        compBuffer[pos] = MESSAGE_REMOTE_END;
		}

	  	if(msgSize > 0){
			Serial.printf("Appending to public log.\n");
 			pfs.fileAppend(pfs_file_publicLog, (uint8_t *)compBuffer, msgSize+2);
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
				sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, p_id);
				pfs.fd = SD.open(fileNameBuffer, FILE_WRITE, O_TRUNC);
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
				break;
                	//keyshareCode
		       	case 0xfaaf3200:
				sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, p_id);
                                Serial.printf("(keyshare) Message Queued to '%s'\n", fileNameBuffer);
                                pfs.fd = SD.open(fileNameBuffer, FILE_WRITE, O_TRUNC);
                                pfs.clearCompBuffer();
                                compBuffer[0] = 0x00;
                                compBuffer[1] = keyshareCode[2];
                                compBuffer[2] = keyshareCode[1];
                                compBuffer[3] = keyshareCode[0];

                                compBuffer[4] = p_id;
                                compBuffer[5] = frameCount;
                                compBuffer[6] = p_size;
                                pfs.fd.write((uint8_t *)compBuffer, 7);

                                //pfs.addToFiledata(buf, bufSize);
                                pfs.fd.write((const uint8_t *)buf, bufSize);
                                pfs.fd.close();
                                Serial.printf("Wrote message to send queue folder.\n");
				break;
                	//encryptedCode
			case 0xe9c74900:
				sprintf(fileNameBuffer, "%s/%d.queued", pfs_folder_sendQueue, p_id);
                                pfs.fd = SD.open(fileNameBuffer, FILE_WRITE, O_TRUNC);
                                pfs.clearFileBuffer();
                                fileData[0] = 0x00;
                                fileData[1] = encryptedCode[2];
                                fileData[2] = encryptedCode[1];
                                fileData[3] = encryptedCode[0];

                                fileData[4] = p_id;
                                fileData[5] = frameCount;
                                fileData[6] = p_size;
                                pfs.fd.write(fileData, 7);

                                pfs.addToFiledata(buf, bufSize);
                                pfs.fd.write(fileData, bufSize);
                                pfs.fd.close();
				break;
			//confirmCode
			case 0xaabdee00:
				fileData[0] = 0x00;
                                fileData[1] = confirmCode[2];
                                fileData[2] = confirmCode[1];
                                fileData[3] = confirmCode[0];
				sendMessage(fileData, bufSize); /*:O going in here transfers control to the other device*/
				break;
                	//doneCode
			case 0xd0439000:
				fileData[0] = 0x00;
                                fileData[1] = doneCode[2];
                                fileData[2] = doneCode[1];
                                fileData[3] = doneCode[0];
                                sendMessage(fileData, bufSize);
				break;
			//okayCode
			case 0xa0ca7100:
				sprintf(compBuffer, "%s/%d.queued", pfs_folder_sendQueue, fileData[4]);
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
		if(previousId != emitPacket->p_id){
			previousId = emitPacket->p_id;
			previousFrameCount = emitPacket->p_count;
			timeout = RADIO_PACKET_TIMEOUT;
		}else if(previousId == emitPacket->p_id && previousFrameCount != emitPacket->p_count){
			timeout = RADIO_PACKET_TIMEOUT;
			previousFrameCount = emitPacket->p_count;
		}

		Serial.printf("Sending Message %x of size %ld (%d)\n", emitPacket->p_code, fileSize, timeout);
		sendMessage(emitBuffer, fileSize);
		if(timeout <= 0 /*|| radioPacket->p_code == okayCode_int*/){
			pfs.rm((const char *)fileNameBuffer);
			timeout = -1;
			previousId = -1;
		}else if(timeout > 0){
			timeout--;
		}

		toggleRecvMode();
		this->resetRadioPacket();
	}

}palcomRadio;
