enum PalcomObjectType{pal_label, pal_textarea, pal_button};
class PalcomObject{
	private:
		lv_obj_t *object = NULL;
		int id = -1;
	public:
		lv_obj_t *getObject(void){
			return this->object;
		}
		void setObject(lv_obj_t *object){
			this->object = object;
		}

		int getObjectId(void){
			return id;
		}

		void setObjectId(int id){
			this->id = id;
		}

		void destroy(void){
			if(this->object != NULL){
                                lv_obj_del(this->object);
                                this->object = NULL;
                        }
		}

		void generate(lv_obj_t *parent, PalcomObjectType objtype){
			switch(objtype){
				case pal_label:
					this->object = lv_label_create(parent);
					break;
				case pal_textarea:
					this->object = lv_textarea_create(parent);
					lv_obj_add_state(this->object, LV_STATE_FOCUSED);
					lv_obj_add_state(this->object, LV_STATE_DEFAULT);
					break;
				case pal_button:
					this->object = lv_btn_create(parent);
					break;
			}
		}

		void generateGlobal(lv_obj_t *parent, uint id, PalcomObjectType objtype){
                        if(id >= GLOBAL_GUI_OBJECT_COUNT){
                                return;
                        }
                        this->id = id;
			switch(objtype){
                        	case pal_label: 
					globalGuiObjects[this->id] = lv_label_create(parent);
					break;
				case pal_textarea:
					globalGuiObjects[this->id] = lv_textarea_create(parent);
					break;
				case pal_button:
					globalGuiObjects[this->id] = lv_btn_create(parent);
					break;
			}
                        this->object = globalGuiObjects[this->id];
                }

		void loadGlobal(int id){
                        this->object = globalGuiObjects[id];
                }

		void setDefaultStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_DEFAULT);
		}

		void setCheckedStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_CHECKED);
		}

		void setFocusedStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_FOCUSED);
		}

		void setFocusedKeyStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_FOCUS_KEY);
		}

		void setEditedStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_EDITED);
		}

		void setHoveredStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_HOVERED);
		}

		void setPressedStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_PRESSED);
		}

		void setScrolledStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_SCROLLED);
		}

		void setDisabledStyle(lv_style_t *s){
			lv_obj_add_style(this->object, s, LV_STATE_DISABLED);
		}
};
