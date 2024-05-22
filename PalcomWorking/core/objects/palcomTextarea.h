#define PALCOM_TA_CURSOR_UP 0
#define PALCOM_TA_CURSOR_DOWN 1
#define PALCOM_TA_CURSOR_LEFT 2
#define PALCOM_TA_CURSOR_RIGHT 3
class PalcomTextarea : public PalcomObject{
  	private:
  		lv_style_t style;
		lv_style_t focusedStyle;
	public:
    		lv_obj_t *getObj(void){
      			return this->getObject();
    		}

		void pushChar(char c){
			lv_textarea_add_char(this->getObject(), c);
		}

		void popCharLeft(void){
			lv_textarea_del_char(this->getObject());
		}
		void popCharRight(void){
			lv_textarea_del_char_forward(this->getObject());
		}

		void pushText(const char *t){
			lv_textarea_add_text(this->getObject(), t);
		}

    		void setText(const char *text){
      			lv_textarea_set_text(this->getObject(), text);
    		}
		void setTextPlaceholder(const char *t){
			lv_textarea_set_placeholder_text(this->getObject(), t);
		}

		void setStyle(lv_style_t *style, lv_style_t *focusedStyle){
			this->setDefaultStyle(style);
			this->setFocusedStyle(focusedStyle);
                }

		void setStyleMain(lv_style_t *style){
			this->setAnyStyle(style, LV_PART_MAIN);
		}

		void setStyleScrollbar(lv_style_t *style){
			this->setAnyStyle(style, LV_PART_SCROLLBAR);
		}

		void setStyleSelected(lv_style_t *style){
			this->setAnyStyle(style, LV_PART_SELECTED);
		}

		void setStyleCursor(lv_style_t *style){
			this->setAnyStyle(style, LV_PART_CURSOR);
		}

		void setStylePlaceholder(lv_style_t *style){
			this->setAnyStyle(style, LV_PART_TEXTAREA_PLACEHOLDER);
		}

    		void create(lv_obj_t *parent){
			this->generate(parent, pal_textarea);
			//lv_obj_remove_style_all(this->getObject());
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

		void moveCursor(int direction){
			switch(direction){
				case PALCOM_TA_CURSOR_UP:
					lv_textarea_cursor_up(this->getObject());
					break;
				case PALCOM_TA_CURSOR_DOWN:
					lv_textarea_cursor_down(this->getObject());
					break;
				case PALCOM_TA_CURSOR_LEFT:
					lv_textarea_cursor_left(this->getObject());
					break;
				case PALCOM_TA_CURSOR_RIGHT:
					lv_textarea_cursor_right(this->getObject());
					break;
			}
		}

		void setValidChars(const char *s){
			lv_textarea_set_accepted_chars(this->getObject(), s);
		}
};
