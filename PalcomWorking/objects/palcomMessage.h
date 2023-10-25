typedef struct pal_message_struct{
	uint32_t msgCount = 0;
	char *name = NULL;
	
}pmsg_t;
lv_style_t recievedMessage;
lv_style_t sentMessage;
class PalcomMessage{
	private:
		lv_obj_t *objectBackground = NULL;
		lv_obj_t *object = NULL;
    		int objectId = -1;
		PalcomLabel pLabel;


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

		void loadEncryptedMessages(int friendIndex){
			createMessage(0, false, 0x888, "Joe:\nniggers.");
			createMessage(1, true, 0x222, "Me:\n>:(");
		}

		void loadGeneralMessages(void){
			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
				fileData[i] = 0;
				compBuffer[i] = 0;
			}
			PalcomFS pfs; 
			size_t pubMsgSize = pfs.getPublicMessages(); // public messages stored in fileData
			int msgIndex =0;
			bool mine = false;
			uint32_t color = 0x555;
			size_t messageSize = 0;
			for(int i=0;i<pubMsgSize && i <__GLOBAL_BUFFER_SIZE; i++){
				if(i+5 >= pubMsgSize){
					break;
				}else if(fileData[i] == '\n' && fileData[i+1] == '\t' && fileData[i+2] == ' '){
					fileData[i] = 0x00;
					if(fileData[i+3] == '0'){
						mine = true;
						color = 0x222;
					}else{
						mine = false;
						color = 0x888;
					}
					for(int j=0; j<messageSize && j < __GLOBAL_BUFFER_SIZE; j++){
						compBuffer[j] = fileData[(i-messageSize)+j];
					}

					createMessage(msgIndex, mine, color, (const char *)compBuffer);
					lv_task_handler();
					messageSize = 0;
					msgIndex++;
					i+=4;
					// End of message.
				}else{
					compBuffer[messageSize] = fileData[i];
					messageSize++;
				}
			}
			if(messageSize > 0){
				compBuffer[messageSize] = 0x0;
				createMessage(msgIndex, mine, color, (const char *)compBuffer);
                                lv_task_handler();
			}

			lv_obj_scroll_to_y(this->objectBackground, LV_COORD_MAX, LV_ANIM_OFF);
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
