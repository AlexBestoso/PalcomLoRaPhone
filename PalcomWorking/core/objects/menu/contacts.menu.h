bool exitContactsMenu = false;
struct palcom_contact{
	lv_obj_t *name = NULL;
	lv_obj_t *number = NULL;
};typedef struct palcom_contact contact_t;

PalcomTextarea contactsNameTxtArea;
PalcomTextarea contactsNumberTxtArea;
bool contacts_redisplay = false;
String contact_selected = "";
class ContactsMenu : public PalcomMenu{
	private:
		PalcomPage rootPage;
		PalcomPage addPage;
		PalcomPage selectedPage;

		File root;

		int contactIdStorage[256];

		static void exitFunc(lv_event_t *e){
			PalcomEvent event(e);
			lv_obj_t *menu = (lv_obj_t *)event.getUserData();
			lv_obj_t *obj = (lv_obj_t *)event.getTarget();
			if(event.getCode() == LV_EVENT_CLICKED && lv_menu_back_button_is_root(menu, obj)){
				exitContactsMenu = true;
			}
		}

		static void addContactFunc(lv_event_t *e){
			PalcomEvent event(e);
			//contact_t *data = (contact_t *)event.getUserData();
			lv_obj_t *obj = (lv_obj_t *)event.getTarget();
			if(event.getCode() == LV_EVENT_RELEASED){
				Serial.printf("Adding contact...\n");
				String name = contactsNameTxtArea.getText(); 
				String number = contactsNumberTxtArea.getText();
				if(number == "" || number.length() != 11){
					contactsNumberTxtArea.setStyleBgColor(contactsNameTxtArea.colors.make(255, 0, 0));
					return;
				}
				
				contacts_redisplay = true;
				contactsNumberTxtArea.setStyleBgColor(contactsNameTxtArea.getStyleBgColor());
				contactsNameTxtArea.setText("");
				contactsNumberTxtArea.setText("");

				PalcomFS pfs;
				if(!SD.exists(pfs_dir_contacts)){
					Serial.printf("Created contact folder.\n");
					SD.mkdir(pfs_dir_contacts);
				}
				
				sprintf((char*)&fileData, "%s/%s", pfs_dir_contacts, number.c_str());
				if(!SD.exists((const char *)fileData)){
					Serial.printf("Added number to contact.\n");
					SD.mkdir((const char *)fileData);
				}

				if(name != ""){
					sprintf((char *)compBuffer, "%s", name.c_str());
					sprintf((char*)&fileData, "%s/name", fileData);
					pfs.fd = SD.open((const char*)fileData, FILE_WRITE, O_TRUNC);
					pfs.fd.write((unsigned char *)compBuffer, name.length());
					pfs.close();
					Serial.printf("Added name to contact.\n");
				}
			}
		}

		static void addContactButtFunc(lv_event_t *e){
			PalcomEvent event(e);
			lv_obj_t *data = (lv_obj_t *)event.getUserData();
			lv_obj_t *obj = (lv_obj_t *)event.getTarget();
			if(event.getCode() == LV_EVENT_RELEASED){
				Serial.printf("Show add contact form.\n");	
			}
		}

		static void contactMessage(lv_event_t *e){
			PalcomEvent event(e);
			uint8_t *targetDir = (uint8_t *)event.getUserData();
			if(event.getCode() == LV_EVENT_RELEASED){
				int val = *targetDir;
				contacts_redisplay = true;
                                int contactCount = 0;
                                File root = SD.open(pfs_dir_contacts);
                                while(true){
                                        File node = root.openNextFile();
                                        if(!node){
                                                break;
                                        }
                                        if(node.isDirectory()){
                                                if(contactCount != val){
                                                        contactCount++;
                                                        node.close();
                                                        continue;
                                                }
                                                String name = node.name();
                                                sprintf((char *)fileData, "%s/%s", pfs_dir_contacts, name.c_str());
						contact_selected = (const char *)fileData;
						Serial.printf("Switching to message contact view for '%s'\n", contact_selected.c_str());
                                            	
                                        }
                                        node.close();
                                        contactCount++;
                                }
                                root.close();
			}
		}

		static void contactDelete(lv_event_t *e){
			PalcomEvent event(e);
			uint8_t* targetDir = (uint8_t *)event.getUserData();
			lv_obj_t *obj = (lv_obj_t *)event.getTarget();
			if(event.getCode() == LV_EVENT_RELEASED){
				int val = *targetDir;
				contacts_redisplay = true;
                        	int contactCount = 0;
				File root = SD.open(pfs_dir_contacts);
                        	while(true){
                        	        File node = root.openNextFile();
                        	        if(!node){
                        	                break;
                        	        }
                        	        if(node.isDirectory()){
						if(contactCount != val){
							contactCount++;
							node.close();
							continue;
						}
                        	                String name = node.name();
						sprintf((char *)fileData, "%s/%s", pfs_dir_contacts, name.c_str());
						PalcomFS pfs;
						pfs.rm((const char *)fileData);
					}
					node.close();
					contactCount++;
				}
				root.close();
			}
		}

		void createAddPage(void){
			addPage.create(this->getObject(), NULL);
			addPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
			addPage.addSeparator();
			addPage.section.create(addPage.getObject());

			lv_obj_t *obj = this->create_text(addPage.section.getObject(), LV_SYMBOL_CALL, "Phone Number", 2);
			PalcomTextarea textarea;
			contactsNumberTxtArea.create(obj);
			contactsNumberTxtArea.setSize(120, 40);
			contactsNumberTxtArea.setMaxLength(11);
			contactsNumberTxtArea.setNumbersOnly();
			contactsNumberTxtArea.setOneLine(true);
			this->execute();

			obj = this->create_text(addPage.section.getObject(), LV_SYMBOL_SETTINGS, "Name", 2);
                        contactsNameTxtArea.create(obj);
                        contactsNameTxtArea.setSize(120, 40);
			contactsNameTxtArea.setMaxLength(20);
			contactsNameTxtArea.setOneLine(true);


			obj = this->create_button(addPage.section.getObject(), LV_SYMBOL_SETTINGS, "Add", &addContactFunc);
		}


		 void createContactsList(lv_obj_t *cont){
			int fileIdx = 0;
			PalcomFS pfs;
			pfs.clearCompBuffer();
			String number = "";
			String name = "";
			uint8_t contactId = 0;
			for(int i=0; fileData[i] != '~'; i++){
				if(i>=__GLOBAL_BUFFER_SIZE){
					break;
				}else if(fileData[i] != ' '){
					compBuffer[fileIdx] = fileData[i];
					fileIdx++;
					continue;
				}
				
				bool processing = false;
				for(int j=0; j<fileIdx; j++){
					if(compBuffer[j] == ':'){
						processing = true;
						continue;
					}
					if(processing){
						name += compBuffer[j];
					}else{
						number += compBuffer[j];
					}
				}
				if(name == "")
					name = "Unknown";

				selectedPage.create(this->getObject(), NULL);
                        	selectedPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
                        	selectedPage.addSeparator();
                        	selectedPage.section.create(selectedPage.getObject());

				String dispTxt = "#: ";
				dispTxt += number.c_str();
				lv_obj_t *obj = this->create_text(selectedPage.section.getObject(), NULL, dispTxt.c_str(), 2);
				this->execute();

				dispTxt = "Name: ";
				dispTxt += name.c_str();
				obj = this->create_text(selectedPage.section.getObject(), NULL, dispTxt.c_str(), 2);

				obj = this->create_button(selectedPage.section.getObject(), NULL, "Message", &contactMessage, (void*)&contactIdStorage[contactId]);
				obj = this->create_button(selectedPage.section.getObject(), NULL, "Delete", &contactDelete, (void*)&contactIdStorage[contactId]);

				// add to side bar.
				rootPage.section.create(rootPage.getObject());
				this->execute();
                        	cont = this->create_text(rootPage.section.getObject(), LV_SYMBOL_CALL, name.c_str(), 1);
				this->execute();
                        	this->linkContainerToPage(cont, selectedPage.getObject());
				this->execute();
				
				pfs.clearCompBuffer();
				name = "";
				number = "";
				fileIdx = 0;
				contactId++;
			}
			printf("");
		}

		lv_obj_t *create_button(lv_obj_t * parent, const char * icon, const char * txt, void(*func)(lv_event_t *e), void *data=NULL){
                        //lv_obj_t * obj = create_text(parent, icon, txt, 2);
                        PalcomButton button;
                        button.create(parent);
                        button.setSize(90, 40);
			if(data == NULL)
	                        button.setSimpleCallback(func);
			else
				button.setParamCallback(func, data, LV_EVENT_RELEASED);
                        PalcomLabel label;
                        label.create(button.getObject());
                        label.setText(txt);
                        label.center();
                        button.setLabel(label);

                        return button.getObject();
                }

		lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt, int builder_variant){
                        lv_obj_t * obj = this->createContainer(parent);

                        lv_obj_t * img = NULL;
                        PalcomLabel label;

                        if(icon) {
                                img = lv_img_create(obj);
                                lv_img_set_src(img, icon);
                        }

                        if(txt) {
                                label.create(obj);
                                label.setText(txt);
                                label.setLongMode(LV_LABEL_LONG_SCROLL_CIRCULAR);
                                label.setFlexGrow(1);
                        }

                        if(builder_variant == 2 && icon && txt) {
                                lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
                                lv_obj_swap(img, label.getObject());
                        }

                        return obj;
                }
	public:
		void reset(void){
			exitContactsMenu = false;
			contact_selected = "";
		}

		bool exitMenu(void){
			return exitContactsMenu;
		}

		bool rebuild(void){
			bool ret = contacts_redisplay;
			contacts_redisplay = false;
			return ret;
		}

		bool contactSelected(void){
			return (contact_selected != "");
		}

		String getSelectedContact(void){
			return contact_selected;
		}
		
		void make(lv_obj_t *parent){
			root = SD.open(pfs_dir_contacts);
			if(!root.isDirectory()){
                                root.close();
                                return;
                        }
			PalcomFS pfs;
			pfs.clearFileBuffer();
			int fileDataIdx = 0;
			int contactCount = 0;
			while(true){
				if(fileDataIdx >= __GLOBAL_BUFFER_SIZE)
					break;
				File node = root.openNextFile();
				this->execute();
				if(!node){
					fileData[fileDataIdx] = '~'; 
					fileDataIdx++;
					break;
				}
				this->execute();
				if(node.isDirectory()){
					String name = node.name();
					for(int i=0; i<name.length(); i++){
						fileData[fileDataIdx] = name[i];
						fileDataIdx++;
					}
					sprintf((char *)compBuffer, "%s/%s/name", pfs_dir_contacts, name.c_str());
					if(SD.exists((const char *)compBuffer)){
						File fd = SD.open(compBuffer, FILE_READ);
						pfs.clearCompBuffer();
						size_t s = fd.size();
						fd.read((unsigned uint8_t *)compBuffer, s);
						fd.close();
						fileData[fileDataIdx] = ':';
						fileDataIdx++;
						for(int i=0; i<s; i++){
							fileData[fileDataIdx] = compBuffer[i];
							fileDataIdx++;
						}
					}

					fileData[fileDataIdx] = ' ';
					fileDataIdx++;
					contactIdStorage[contactCount] = contactCount;
					contactCount++;
					if(contactCount >= 256)
						break;
				}
				node.close();
			}
			root.close();


			this->create(parent);
			this->execute();

			lv_color_t bgColor = this->getStyleBgColor();
			if(this->colors.getBrightness(bgColor) > 127){
				this->setStyleBgColor(this->colors.darken(bgColor, 10));
			}else{
				this->setStyleBgColor(this->colors.darken(bgColor, 50));
			}

			this->enableRootBackButton(true);
			this->setParamCallback(exitFunc, (void*)this->getObject(), LV_EVENT_CLICKED);
			this->fullScreen();
			this->center();
			this->execute();

			this->createAddPage();
			this->execute();

			/*
			 * 
			 * */

			rootPage.create(this->getObject(), "Back");
			rootPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
			this->execute();

			lv_obj_t * cont; 
			rootPage.section.create(rootPage.getObject());
                        cont = this->create_text(rootPage.section.getObject(), LV_SYMBOL_SETTINGS, "Add", 1);
                        this->linkContainerToPage(cont, addPage.getObject());
			this->execute();

			this->createContactsList(cont);
			this->execute();

			this->setSidebarPage(rootPage.getObject());
			lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(this->getObject()), 0), 0), LV_EVENT_CLICKED,NULL);
			this->execute();
		}
};
