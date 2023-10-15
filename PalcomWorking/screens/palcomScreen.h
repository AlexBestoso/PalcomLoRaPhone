
class PalcomScreen{
  private:
    lv_obj_t *screen = NULL;
    int objectId = -1;
  public:
    void create(void){
      this->screen = lv_scr_act();
      //lv_scr_load(this->screen);
    }

    void createGlobal(void){
      lv_obj_clean(lv_scr_act());
      globalGuiObjects[0] = lv_scr_act();
      objectId = 0;
      screen = globalGuiObjects[0];
    }

    void destroy(void){
      lv_obj_clean(lv_scr_act());
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

    void setBgImage(const lv_img_dsc_t *img[1]){
      lv_img_set_src(lv_img_create(this->screen), (void *)(img[0]));
    }
    virtual void generateObjects(){

    }
};