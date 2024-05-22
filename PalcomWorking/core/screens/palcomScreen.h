int PalcomScreenError = 0;
class PalcomScreen{ 
  	private:
    		lv_obj_t *screen = NULL;
		lv_obj_t *bgImage = NULL;
    		int objectId = -1;
		bool buildRequired = true;
  	public:

		void clearScreenError(void){
			PalcomScreenError = 0;
		}

		bool isScreenError(void){
			return (PalcomScreenError == 0) ? false : true;
		}

		void setScreenError(int e){
			PalcomScreenError = e;
		}

		int getScreenError(){
			int ret = PalcomScreenError;
			PalcomScreenError = 0;
			return ret;
		}
		bool getBuildRequired(void){
			return buildRequired;
		}
		void setBuildRequired(bool v){
			buildRequired = v;
		}

    		void create(void){
      			this->screen = lv_scr_act();
    		}

    		void createGlobal(void){
      			lv_obj_clean(lv_scr_act());
      			globalGuiObjects[0] = lv_scr_act();
      			objectId = 0;
      			screen = globalGuiObjects[0];
    		}

    		void destroy(void){
      			lv_obj_clean(lv_scr_act());
			this->screen = NULL;
			this->bgImage = NULL;
    		}

    		void globalDestroy(void){
      			for(int i=0; i<GLOBAL_GUI_OBJECT_COUNT; i++){
        			if(globalGuiObjects[i] != NULL){
        				lv_obj_del(globalGuiObjects[i]);
          				globalGuiObjects[i] = NULL;
        			}
      			}
    		}

    		void load(void){
      			generateObjects();
    		}

    		lv_obj_t *getScreen(void){
      			return screen;
    		}

    		void setFullScreen(void){
      			lv_obj_set_size(this->screen, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
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
      			lv_obj_set_scroll_dir(this->screen, direction);
    		}
		/*
		 * Scrollbars are displayed according to a configured mode. The following mode(s) exist:
    		* LV_SCROLLBAR_MODE_OFF: Never show the scrollbars
    		* LV_SCROLLBAR_MODE_ON: Always show the scrollbars
    		* LV_SCROLLBAR_MODE_ACTIVE: Show scroll bars while an object is being scrolled
    		* LV_SCROLLBAR_MODE_AUTO: Show scroll bars when the content is large enough to be scrolled
		*/
		void setScrollMode(lv_scrollbar_mode_t mode){
			lv_obj_set_scrollbar_mode(this->screen, mode);
		}

    		void setBgImage(const lv_img_dsc_t *img[1]){
			if(this->bgImage==NULL)
				this->bgImage = lv_img_create(this->screen);
      			lv_img_set_src(this->bgImage, (void *)(img[0]));
    		}

		void setBgX(int offset){
			if(this->bgImage==NULL)
				this->bgImage = lv_img_create(this->screen);
			lv_img_set_offset_x(this->bgImage, offset);
		}

		void setBgY(int offset){
                        if(this->bgImage==NULL)
                                this->bgImage = lv_img_create(this->screen);
                        lv_img_set_offset_y(this->bgImage, offset);
                }
    
		void execute(){
			lv_task_handler();
		}

		virtual void generateObjects(){

    		}

		void setFlag(lv_obj_flag_t f){
                        lv_obj_add_flag(this->screen, f);
                }

                void unsetFlag(lv_obj_flag_t f){
                        lv_obj_clear_flag(this->screen, f);
                }
};
