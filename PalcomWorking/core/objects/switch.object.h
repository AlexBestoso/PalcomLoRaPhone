class PalcomSwitch : public PalcomObject{
	private:

	public:
		void create(lv_obj_t *parent){
			this->generate(parent, pal_switch);
		}

		void addDisabled(void){
			this->addState(LV_STATE_DISABLED);
		}

		void addChecked(void){
			this->addState(LV_STATE_CHECKED);
		}

		void removeDisabled(void){
                        this->removeState(LV_STATE_DISABLED);
                }

                void removeChecked(void){
                        this->removeState(LV_STATE_CHECKED);
                }

};
