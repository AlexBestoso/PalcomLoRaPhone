class PalcomLabel : public PalcomObject{
  	private:
	public:
    		lv_obj_t *getObj(void){
      			return this->getObject();
    		}

    		void create(lv_obj_t *parent){
			this->generate(parent, pal_label);
    		}

    		void createGlobal(lv_obj_t *parent, uint id){
			this->generateGlobal(parent, id, pal_label);
    		}

		/*
    		 * Valid Modes : 
    		 * LV_LABEL_LONG_EXPAND, LV_LABEL_LONG_BREAK, LV_LABEL_LONG_DOT,
    		 * LV_LABEL_LONG_SCROLL, LV_LABEL_LONG_SCROLL_CIRC, LV_LABEL_LONG_CROP
    		 */
    		void setLongMode(int mode){
      			lv_label_set_long_mode(this->getObject(), mode);
    		}

    		void setWidth(int width){
      			lv_obj_set_width(this->getObject(), width);
    		}

    		void setText(const char *text){
      			lv_label_set_text(this->getObject(), text);
    		}

		const char *getText(void){
			return (const char *)lv_label_get_text(this->getObject());
		}

};
