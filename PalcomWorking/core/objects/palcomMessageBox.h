class PalcomMessageBox : public PalcomObject{
	private:

	public:
		void create(lv_obj_t *parent, const char *title, const char *text, bool closeButton){
			this->setObject(lv_msgbox_create(parent));//, title, text, btnTexts, closeButton));
			this->setTitle(title);
			this->setMsgText(text);
			if(closeButton)
				this->setCloseButton();
                        //lv_obj_remove_style_all(this->getObject());
                }

		void setTitle(const char *t){
			lv_msgbox_add_title(this->getObject(), t);
		}

		void setMsgText(const char *t){
			lv_msgbox_add_text(this->getObject(), t);
		}

		void setFooterButton(const char *name, void (*func)(lv_event_t *), int btnId){
			lv_obj_t *b = lv_msgbox_add_footer_button(this->getObject(), name);
			PalcomButton btn;
			btn.setObject(b);
			btn.setParamCallback(func, (void *)&btnId);

		}

		void setCloseButton(void){
			lv_msgbox_add_close_button(this->getObject());
		}


		bool uninitalized(void){
			if(this->getObject() == NULL)
				return true;
			return false;
		}

};
