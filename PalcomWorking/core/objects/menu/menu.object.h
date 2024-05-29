class PalcomMenu : public PalcomObject{
  	private:
    		PalcomLabel label;

		PalcomButton menuBackButton;
		PalcomLabel menuBackButtonLabel;

		lv_style_t style;
		lv_style_t pressedStyle;


	public:
		PalcomPage page;

		lv_obj_t *rootPage;

		void create(lv_obj_t *parent){
      			this->generate(parent, pal_menu);
    		}

		int getMainHeaderLeftPadding(void){
			return lv_obj_get_style_pad_left(getMainHeader(), 0);
		}

		void setSubPageSize(lv_obj_t *page, int x, int y){
			lv_obj_set_size(page, x, y);
		}

		lv_obj_t *createPage(void){
			return lv_menu_page_create(this->getObject(), NULL);
		}

		lv_obj_t *newPage(const char *title){
			return lv_menu_page_create(this->getObject(), (char *)title);
		}

		lv_obj_t *createContainer(lv_obj_t *o){
			return lv_menu_cont_create(o);
		}

		lv_obj_t *createSection(lv_obj_t *o){
			return lv_menu_section_create(o);
		}

    		void createGlobal(lv_obj_t *parent, int id){
			this->generateGlobal(parent, id, pal_menu);
    		}

		void setMenuBackButtonText(string text){
			menuBackButton.create(lv_menu_get_main_header_back_button(this->getObject()));
			menuBackButtonLabel.create(menuBackButton.getObject());
			menuBackButtonLabel.setText(text.c_str());
		}

		lv_style_t getStyle(void){
			return this->style;
		}

    		void setLabel(PalcomLabel label){
      			this->label = label;
    		}

		void enableRootBackButton(bool v){
			lv_menu_set_mode_root_back_button(this->getObject(), (v) ? LV_MENU_ROOT_BACK_BUTTON_ENABLED : LV_MENU_ROOT_BACK_BUTTON_DISABLED);
		}

		void setHeaderMode(lv_menu_mode_header_t m){
			lv_menu_set_mode_header(this->getObject(), m);
		}

		void setHMTopFixed(void){
			this->setHeaderMode(LV_MENU_HEADER_TOP_FIXED);
		}

		void setHMTopUnfixed(void){
			this->setHeaderMode(LV_MENU_HEADER_TOP_UNFIXED);
		}
		
		void setHMBottomFixed(void){
			this->setHeaderMode(LV_MENU_HEADER_BOTTOM_FIXED);
		}
    
    		void setValuedCallback(void(*func)(lv_event_t*), uint8_t *val){
      			lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_ALL, val);
    		}
    	
		void setRelativeAlignment(int ref, int x, int y){
      			lv_obj_align_to(this->getObject(), this->label.getObject(), ref, x,  y);
    		}

		void setMainPage(lv_obj_t *o){
			lv_menu_set_page(this->getObject(), o);
		}

		void linkContainerToPage(lv_obj_t *cont, lv_obj_t *page){
			lv_menu_set_load_page_event(this->getObject(), cont, page);
		}

		lv_obj_t *getMainBackButton(void){
			return lv_menu_get_main_header_back_button(this->getObject());
		}

		lv_obj_t *getMainHeader(void){
			return lv_menu_get_main_header(this->getObject());
		}

		int getMainHeaderPaddingLeft(void){
                        return lv_obj_get_style_pad_left((const lv_obj_t *)getMainHeader(), LV_PART_ANY);
                }

		void setSidebarPage(lv_obj_t *o){
			lv_menu_set_sidebar_page(this->getObject(), o);
		}
};
