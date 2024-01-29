typedef struct pal_message_struct{
	uint32_t msgCount = 0;
	char *name = NULL;
	
}pmsg_t;
lv_style_t recievedMessage;
lv_style_t sentMessage;
class PalcomMessage : public PalcomObject{
	private:
		lv_obj_t *objectBackground = NULL;
		lv_obj_t *object = NULL;
    		int objectId = -1;
		PalcomLabel pLabel;
		
		// New variables
		PalcomButton loadMessagesButton;
		
		int maxMessages = 10;

		int messageContainerW = 0;
		int messageContainerH = 0;
		int messageContainerX = 0;
		int messageContainerY = 0;

	public:
		int backgroundW = 300;
		int backgroundH = 120;
		int backgroundX = -5;
		int backgroundY = 0;
		
		int textX = 10;
		int textY = -20;
		int textRef = LV_ALIGN_OUT_BOTTOM_MID;

		int messageW = 300;
		int messageH = 70;
		int messageX = 0;
		int messageY = 0;
		bool messageForign = false;

		lv_obj_t *getObj(void){
      			return object;
    		}

		void createBackground(lv_obj_t *parent){
			this->objectBackground = lv_obj_create(parent);
			lv_obj_set_size(this->objectBackground, backgroundW, backgroundH);
			lv_obj_set_pos(this->objectBackground, backgroundX, backgroundY);
		}

		void create(lv_obj_t *parent){
			this->generate(parent, pal_base);
			this->setSize(this->messageContainerW, this->messageContainerH);
			this->setAlignment(LV_ALIGN_OUT_TOP_LEFT, this->messageContainerX, this->messageContainerY);
			lv_style_init(&recievedMessage);
                        lv_style_set_bg_color(&recievedMessage, lv_color_hex(0xaaaaaa));
			this->setDefaultStyle(&recievedMessage);
		}

		void setContainerW(int w){
			this->messageContainerW = w;
		}
		
		void setContainerH(int h){
                	this->messageContainerH = h;
		}
		
		void setContainerX(int x){
                	this->messageContainerX = x;
		}
		
		void setContainerY(int y){
                	this->messageContainerY = y;
		}

    		void create(){
			if(objectBackground == NULL){
				return ;
			}
      			this->object = lv_obj_create(objectBackground);
			lv_obj_set_size(this->object, messageW, messageH);
			lv_obj_set_pos(this->object, messageX, messageY);
    		}

		void createMessage(int index, bool mine, uint32_t color, const char *msg){
			if(!mine){
                        	lv_style_init(&recievedMessage);
                        	lv_style_set_bg_color(&recievedMessage, lv_color_hex(color));
				
				this->object = lv_obj_create(objectBackground);
                        	lv_obj_set_size(this->object, messageW, messageH);
                        	lv_obj_set_pos(this->object, 0, messageY + (index*(messageH+3)));

                        	lv_obj_add_style(object, &recievedMessage, LV_PART_MAIN);
			}else{
                        	lv_style_init(&sentMessage);
                        	lv_style_set_bg_color(&sentMessage, lv_color_hex(color));
				
				this->object = lv_obj_create(objectBackground);
                        	lv_obj_set_size(this->object, messageW, messageH);
                        	lv_obj_set_pos(this->object, messageW+1, messageY + (index*(messageH+3)));

                        	lv_obj_add_style(object, &sentMessage, LV_PART_MAIN);
			}
			
			
			pLabel.create(object);
			pLabel.setText(msg);
		}

		void loadEncryptedMessages(string friendHash){
			Serial.printf("Loadnig friend messages...\n");
			PalcomFS pfs;
			pfs.clearFileBuffer();
			size_t pubMsgSize = pfs.getFriendMessages(friendHash); // public messages stored in fileData
			pfs.clearCompBuffer();
			Serial.printf("Message Size : %ld\n", pubMsgSize);
			int msgIndex =0;
			bool mine = false;
			uint32_t color = 0x555;
			size_t messageSize = 0;
			bool found = false;
			for(int i=0;i<pubMsgSize && i <__GLOBAL_BUFFER_SIZE; i++){
				if(i >= pubMsgSize){
					break;
				}else if(fileData[i] == (char)MESSAGE_LOCAL_START || fileData[i] == (char)MESSAGE_REMOTE_START){
					found = true;
					continue;
				}else if(fileData[i] == (char)MESSAGE_LOCAL_END || fileData[i] == (char)MESSAGE_REMOTE_END){
					if(fileData[i] == (char)MESSAGE_LOCAL_END){
						mine = true;
						color = 0x222;
					}else{
						mine = false;
						color = 0x888;
					}

					Serial.printf("creating message.\n");
					createMessage(msgIndex, mine, color, (const char *)compBuffer);
					lv_task_handler();
					messageSize = 0;
					msgIndex++;
					found = false;
					pfs.clearCompBuffer();
					// End of message.
				}else if(found){
					compBuffer[messageSize] = fileData[i];
					messageSize++;
				}
			}
			if(messageSize > 0){
				compBuffer[messageSize+2] = 0x0;
				createMessage(msgIndex, mine, color, (const char *)compBuffer);
                                lv_task_handler();
			}

			lv_obj_scroll_to_y(this->objectBackground, LV_COORD_MAX, LV_ANIM_OFF);
                        lv_task_handler();
		}

		void loadGeneralMessages(void){
			PalcomFS pfs; 
			pfs.clearFileBuffer();
			pfs.clearCompBuffer();
			size_t pubMsgSize = pfs.getPublicMessages(); // public messages stored in fileData
			int msgIndex =0;
			bool mine = false;
			uint32_t color = 0x555;
			size_t messageSize = 0;
			bool found = false;
			for(int i=0;i<pubMsgSize && i <__GLOBAL_BUFFER_SIZE; i++){
				if(i >= pubMsgSize){
					break;
				}else if(fileData[i] == (char)MESSAGE_LOCAL_START || fileData[i] == (char)MESSAGE_REMOTE_START){
					found = true;
					continue;
				}else if(fileData[i] == (char)MESSAGE_LOCAL_END || fileData[i] == (char)MESSAGE_REMOTE_END){
					if(fileData[i] == (char)MESSAGE_LOCAL_END){
						mine = true;
						color = 0x222;
					}else{
						mine = false;
						color = 0x888;
					}

					createMessage(msgIndex, mine, color, (const char *)compBuffer);
					lv_task_handler();
					messageSize = 0;
					msgIndex++;
					found = false;
					pfs.clearCompBuffer();
					// End of message.
				}else if(found){
					compBuffer[messageSize] = fileData[i];
					messageSize++;
				}
			}
			if(messageSize > 0){
				compBuffer[messageSize+2] = 0x0;
				createMessage(msgIndex, mine, color, (const char *)compBuffer);
                                lv_task_handler();
			}

			lv_obj_scroll_to_y(this->objectBackground, LV_COORD_MAX, LV_ANIM_OFF);
                        lv_task_handler();
		}
	
    		void createGlobal(lv_obj_t *parent, uint id){
      			if(id >= GLOBAL_GUI_OBJECT_COUNT){
      			  	return;
      			}
      			objectId = id;
      			globalGuiObjects[id] = lv_obj_create(parent);
      			this->objectBackground = globalGuiObjects[id];

			lv_obj_set_size(this->objectBackground, this->backgroundW, this->backgroundH);
			lv_obj_set_pos(this->objectBackground, this->backgroundX, this->backgroundY);
    		}

    		void loadGlobal(int id){
      			object = globalGuiObjects[id];
    		}

		void setRelativeAlignment(int ref, int x, int y){
      			lv_obj_align_to(this->object, this->pLabel.getObj(), ref, x,  y);
    		}
		
    		void destroy(){
      			if(this->object != NULL){
        			lv_obj_del(this->object);
        			this->object = NULL;
      			}
			if(this->objectBackground != NULL){
				lv_obj_del(this->objectBackground);
				this->objectBackground = NULL;
			}
    		}
};
