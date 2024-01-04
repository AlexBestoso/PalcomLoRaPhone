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

    		/*
    		 * Valid References are :
    		 * LV_ALIGN_OUT_TOP_LEFT, LV_ALIGN_OUT_TOP_MID, LV_ALIGN_OUT_TOP_RIGHT, 
    		 * LV_ALIGN_OUT_RIGHT_TOP, LV_ALIGN_OUT_RIGHT_MID, LV_ALIGN_OUT_RIGHT_BOTTOM,
    		 * LV_ALIGN_OUT_BOTTOM_RIGHT, LV_ALIGN_OUT_BOTTOM_MID, LV_ALIGN_OUT_BOTTOM_LEFT,
    		 * LV_ALIGN_OUT_LEFT_BOTTOM, LV_ALIGN_OUT_LEFT_MID, LV_ALIGN_OUT_LEFT_TOP,
    		 * LV_ALIGN_IN_TOP_LEFT, LV_ALIGN_IN_TOP_MID, LV_ALIGN_IN_TOP_RIGHT,
    		 * LV_ALIGN_IN_RIGHT_MID, LV_ALIGN_IN_BOTTOM_RIGHT, LV_ALIGN_IN_BOTTOM_MID,
    		 * LV_ALIGN_IN_BOTTOM_LEFT, LV_ALIGN_IN_LEFT_MID, LV_ALIGN_CENTER
    		 */
    		void setAlignment(int ref, int x, int y){
      			lv_obj_align(this->getObject(), ref, x, y);
    		}

    		void center(void){
      			lv_obj_center(this->getObject());
    		}
};
