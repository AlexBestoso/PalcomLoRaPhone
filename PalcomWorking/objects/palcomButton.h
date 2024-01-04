class PalcomButton : public PalcomObject{
  	private:
    		PalcomLabel label;
		lv_style_t style;
		lv_style_t pressedStyle;

	public:
    		lv_obj_t *getObj(){
      			return this->getObject();
    		}
    	
		void create(lv_obj_t *parent){
      			this->generate(parent, pal_button);
			lv_obj_remove_style_all(this->getObject());
    		}

    		void createGlobal(lv_obj_t *parent, int id){
			this->generateGlobal(parent, id, pal_button);
    		}

		void setStyle(lv_style_t *style, lv_style_t *pressedStyle){
			this->setDefaultStyle(style);
			this->setPressedStyle(pressedStyle);
		}

		lv_style_t getStyle(void){
			return this->style;
		}

    		void setSize(int x, int y){
      			lv_obj_set_size(this->getObject(), LV_PCT(x), LV_PCT(y));
    		}

    		void setLabel(PalcomLabel label){
      			this->label = label;
    		}
    
    		void setSimpleCallback(void(*func)(lv_event_t*)){
      			lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_ALL, 0);
    		}
    
    		void setValuedCallback(void(*func)(lv_event_t*), uint8_t *val){
      			lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_ALL, val);
    		}
    	
		void setRelativeAlignment(int ref, int x, int y){
      			lv_obj_align_to(this->getObject(), this->label.getObject(), ref, x,  y);
    		}
};
