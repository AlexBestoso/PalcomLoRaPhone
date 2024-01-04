class PalcomTextarea : public PalcomObject{
  	private:
  	
	public:
    		lv_obj_t *getObj(void){
      			return this->getObject();
    		}

    		void create(lv_obj_t *parent){
			this->generate(parent, pal_textarea);
    		}

    		void createGlobal(lv_obj_t *parent, uint id){
			this->generateGlobal(parent, id, pal_textarea);
    		}

    		void setCursorClickPos(bool val){
      			lv_textarea_set_cursor_click_pos(this->getObject(), val);
    		}

    		void setTextSelection(bool val){
      			lv_textarea_set_text_selection(this->getObject(), val);
    		}

    		void setSize(int w, int h){
      			lv_obj_set_size(this->getObject(), w, h);
    		}

    		void setText(const char *text){
      			lv_textarea_set_text(this->getObject(), text);
    		}

    		void setMaxLength(int len){
      			lv_textarea_set_max_length(this->getObject(), len);
    		}

    		void setOneLine(bool val){
      			lv_textarea_set_one_line(this->getObject(), val);
    		}

    		void setAlignment(int ref, int x, int y){
      			lv_obj_align(this->getObject(), ref, x, y);
    		}

    		void setPasswordMode(bool val){
      			lv_textarea_set_password_mode(this->getObject(), val);
    		}

    		const char *getText(void){
      			return lv_textarea_get_text(this->getObject());
    		}
};
