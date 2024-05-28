enum PalcomObjectType{pal_label, pal_textarea, pal_button, pal_imgbutton, pal_base, pal_menu, pal_msgbox, pal_checkbox};
class PalcomObject{
	private:
		lv_obj_t *object = NULL;
		int id = -1;
	public:
		PalcomColors colors;

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

		void create(lv_obj_t *parent){
                        this->generate(parent, pal_label);
                }

                void createGlobal(lv_obj_t *parent, uint id){
                        this->generateGlobal(parent, id, pal_label);
                }

		void addState(lv_state_t s){
			lv_obj_add_state(this->object, s);
		}

		void removeState(lv_state_t s){
			lv_obj_clear_state(this->object, s);
		}

		bool stateInUse(lv_state_t s){
                        return lv_obj_has_state(this->object, s);
                }

		void generate(lv_obj_t *parent, PalcomObjectType objtype){
			switch(objtype){
				case pal_label:
					this->object = lv_label_create(parent);
					break;
				case pal_textarea:
					this->object = lv_textarea_create(parent);
					break;
				case pal_button:
					this->object = lv_btn_create(parent);
					break;
				case pal_imgbutton:
					this->object = lv_imagebutton_create(parent);
					break;
				case pal_base:
					this->object = lv_obj_create(parent);
					break;
				case pal_menu:
					this->object = lv_menu_create(parent);
					break;
				case pal_msgbox:
					//this->object = lv_msgbox_create(parent);
					break;
				case pal_checkbox:
					this->object = lv_checkbox_create(parent);
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
					globalGuiObjects[this->id] = lv_imagebutton_create(parent);
					break;
				case pal_base:
					globalGuiObjects[this->id] = lv_obj_create(parent);
					break;
				case pal_menu:
					globalGuiObjects[this->id] = lv_menu_create(parent);
					break;
				case pal_msgbox:
					//globalGuiObjects[this->id] = lv_msgbox_create(parent);
					break;
				case pal_checkbox:
					globalGuiObjects[this->id] = lv_checkbox_create(parent);
					break;	
			}
                        this->object = globalGuiObjects[this->id];
                }

		void loadGlobal(int id){
                        this->object = globalGuiObjects[id];
                }

		void setAnyStyle(lv_style_t *s, lv_style_selector_t sel){
			lv_obj_add_style(this->object, s, sel);
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
                        lv_obj_set_size(this->object, LV_PCT(x), LV_PCT(y));
                }

                void setSizeRaw(int x, int y){
                        lv_obj_set_size(this->object, x, y);
                }

		/*
                 * Direction can be:
                 * LV_DIR_TOP,
                 * LV_DIR_LEFT,
                 * LV_DIR_BOTTOM,
                 * LV_DIR_RIGHT,
                 * LV_DIR_HOR,
                 * LV_DIR_VER,
                 * LV_DIR_ALL
                 */
                void setScreenScrollDirection(int direction){
                        lv_obj_set_scroll_dir(this->object, direction);
                }

		/*
                 * Scrollbars are displayed according to a configured mode. The following mode(s) exist:
                * LV_SCROLLBAR_MODE_OFF: Never show the scrollbars
                * LV_SCROLLBAR_MODE_ON: Always show the scrollbars
                * LV_SCROLLBAR_MODE_ACTIVE: Show scroll bars while an object is being scrolled
                * LV_SCROLLBAR_MODE_AUTO: Show scroll bars when the content is large enough to be scrolled
                */
                void setScrollMode(lv_scrollbar_mode_t mode){
                        lv_obj_set_scrollbar_mode(this->object, mode);
                }

		void center(void){
                        lv_obj_center(this->object);
                }

		int getHorizontalResolution(void){
			return lv_disp_get_hor_res(NULL);
		}

		int getVirticalResolution(void){
			return lv_disp_get_ver_res(NULL);
		}

		void execute(){
                        lv_task_handler();
			lv_timer_handler();
			lv_tick_inc(5);
                }

		void setSimpleCallback(void(*func)(lv_event_t*)){
                        lv_obj_add_event_cb(this->object, func, LV_EVENT_ALL, 0);
                }

		void setParamCallback(void(*func)(lv_event_t *), void *input){
			lv_obj_add_event_cb(this->object, func, LV_EVENT_ALL, input);
		}

		void setFlag(lv_obj_flag_t f){
			lv_obj_add_flag(this->object, f);
		}

		void unsetFlag(lv_obj_flag_t f){
			lv_obj_clear_flag(this->object, f);
		}

		lv_color_t getStyleBgColor(int v){
			return lv_obj_get_style_bg_color(this->object, v);
		}

		void setStyleBgColor(lv_color_t c, int v){
			lv_obj_set_style_bg_color(this->object, c, v);
		}

		void setStylePaddingHor(int padding, lv_style_selector_t sel){
			lv_obj_set_style_pad_hor(this->object, padding, sel);	
		}

		int getStylePaddingLeft(lv_part_t p){
			return lv_obj_get_style_pad_left((const lv_obj_t *)this->getObject(), p);
		}
};
