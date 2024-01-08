enum PalcomObjectType{pal_label, pal_textarea, pal_button, pal_imgbutton, pal_base};
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
				case pal_imgbutton:
					this->object = lv_imgbtn_create(parent);
					break;
				case pal_base:
					this->object = lv_obj_create(parent);
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
				case pal_imgbutton:
					globalGuiObjects[this->id] = lv_imgbtn_create(parent);
					break;
				case pal_base:
					globalGuiObjects[this->id] = lv_obj_create(parent);
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
                        lv_obj_align(this->object, ref, x, y);
                }

		void setSize(int x, int y){
                        lv_obj_set_size(this->getObject(), LV_PCT(x), LV_PCT(y));
                }

                void setSizeRaw(int x, int y){
                        lv_obj_set_size(this->getObject(), x, y);
                }

		void execute(){
                        lv_task_handler();
                }
};
