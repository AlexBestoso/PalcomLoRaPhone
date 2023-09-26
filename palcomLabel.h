class PalcomLabel{
  private:
    lv_obj_t *label = NULL;
    int objectId = -1;
  public:
    lv_obj_t *getObj(void){
      return this->label;
    }

    void create(lv_obj_t *parent){
      this->label = lv_label_create(parent);
    }
    void createGlobal(lv_obj_t *parent, uint id){
      if(id >= GLOBAL_GUI_OBJECT_COUNT){
        return;
      }
      objectId = id;
      globalGuiObjects[id] = lv_label_create(parent);
      this->label = globalGuiObjects[id];
    }

    void destroy(void){
      if(label != NULL){
        lv_obj_del(label);
        label = NULL;
      }
    }
    /*
     * Valid Modes : 
     * LV_LABEL_LONG_EXPAND, LV_LABEL_LONG_BREAK, LV_LABEL_LONG_DOT,
     * LV_LABEL_LONG_SCROLL, LV_LABEL_LONG_SCROLL_CIRC, LV_LABEL_LONG_CROP
     */
    void setLongMode(int mode){
      lv_label_set_long_mode(this->label, mode);
    }

    void setWidth(int width){
      lv_obj_set_width(this->label, width);
    }

    void setText(const char *text){
      lv_label_set_text(this->label, text);
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
      lv_obj_align(this->label, ref, x, y);
    }

    void center(void){
      lv_obj_center(this->label);
    }
};