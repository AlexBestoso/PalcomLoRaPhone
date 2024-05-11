class PalcomMenu : public PalcomObject{
  	private:
    		PalcomLabel label;

		PalcomButton menuBackButton;
		PalcomLabel menuBackButtonLabel;

		lv_style_t style;
		lv_style_t pressedStyle;

	public:
		void create(lv_obj_t *parent){
      			this->generate(parent, pal_menu);
			lv_obj_remove_style_all(this->getObject());
    		}

		void setSubPageSize(lv_obj_t *page, int x, int y){
			lv_obj_set_size(page, x, y);
		}

		lv_obj_t *createPage(void){
			return lv_menu_page_create(this->getObject(), NULL);
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
			menuBackButton.create(lv_menu_get_main_header_back_btn(this->getObject()));
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
			if(v){
				lv_menu_set_mode_root_back_btn(this->getObject(), LV_MENU_ROOT_BACK_BTN_ENABLED);
			}else{
				lv_menu_set_mode_root_back_btn(this->getObject(), LV_MENU_ROOT_BACK_BTN_DISABLED);
			}
		}
    
    		void setBackCallback(void(*func)(lv_event_t*)){
      			lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_CLICKED, this->getObject());
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
};
