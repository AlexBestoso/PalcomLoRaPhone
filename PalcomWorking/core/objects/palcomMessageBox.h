class PalcomMessageBox : public PalcomObject{
	private:

	public:
		void create(lv_obj_t *parent, const char *title, const char *text, const char *btnTexts[], bool closeButton){
			this->setObject(lv_msgbox_create(parent, title, text, btnTexts, closeButton));
                        //lv_obj_remove_style_all(this->getObject());
                }

		bool uninitalized(void){
			if(this->getObject() == NULL)
				return true;
			return false;
		}

		lv_obj_t *getMbTitle(void){
			if(this->uninitalized()){
				throw CoreException("Message Box object not initalized.", 0x01);
			}
			return lv_msgbox_get_title(this->getObject());
		}

		lv_obj_t *getMbCloseButton(void){
			if(this->uninitalized()){
				throw CoreException("Message Box object not initalized.", 0x01);
			}
			return lv_msgbox_get_close_btn(this->getObject());
		}

		lv_obj_t *getMbText(void){
			if(this->uninitalized()){
				throw CoreException("Message Box object not initalized.", 0x01);
			}
			return lv_msgbox_get_text(this->getObject());
		}

		lv_obj_t *getMbBtns(void){
			if(this->uninitalized()){
				throw CoreException("Message Box object not initalized.", 0x01);
			}
			return lv_msgbox_get_btns(this->getObject());
		}
};
