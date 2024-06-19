class PalcomContactsScreen : public PalcomScreen{
	private:
		ContactsMenu contactsMenu;	
	public:
		 void resetPage(void){
                        this->setBuildRequired(true);
                        this->globalDestroy();
                        this->destroy();
                        this->clearScreenError();
                }

		void generateObjects(void){
			lv_obj_t *screen = this->getScreen();
                        if(screen == NULL){
                                this->globalDestroy();
                                this->create();
                                screen = this->getScreen();
                        }
                        this->setFullScreen();
                        this->setScrollMode(LV_SCROLLBAR_MODE_OFF);
                        this->execute();

			contactsMenu.make(screen);
		}

		int run(void){
                        if(this->getBuildRequired()){
                                this->setBuildRequired(false);
                                this->load();
                        }

                        this->execute();
			if(contactsMenu.rebuild()){
				this->resetPage();
				this->setBuildRequired(true);
				return CONTEXT_CONTACTS;
			}

			if(contactsMenu.exitMenu()){
				this->resetPage();
				contactsMenu.reset();
				return CONTEXT_MAINMENU;
			}
			return CONTEXT_CONTACTS;
		}
}contactsScreen;
