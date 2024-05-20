class PalcomCheckbox : public PalcomObject{
	private:

	public:
		void create(lv_obj_t *parent){
			this->generate(parent, pal_checkbox);
		}

		void setText(const char *text){
			lv_checkbox_set_text(this->getObject(), text);
		}

		void setCallback(void(*func)(lv_event_t*)){
			this->setSimpleCallback(func);
		}

		void setChecked(void){
			this->addState(LV_STATE_CHECKED);
		}

		void unsetChecked(void){
			this->removeState(LV_STATE_CHECKED);
		}

		void setDisabled(void){
			this->addState(LV_STATE_DISABLED);
		}

		void unsetDisabled(void){
			this->removeState(LV_STATE_DISABLED);
		}
};
