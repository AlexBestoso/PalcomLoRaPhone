class PalcomButton{
  private:
    lv_obj_t *button;
    int objectId = -1;
    PalcomLabel label;
  public:
    lv_obj_t *getObj(){
      return button;
    }
    void create(lv_obj_t *parent){
      this->button = lv_btn_create(parent);
    }

    void createGlobal(lv_obj_t *parent, uint id){
      if(id >= GLOBAL_GUI_OBJECT_COUNT){
        return;
      }
      objectId = id;
      globalGuiObjects[id] = lv_btn_create(parent);
      this->button = globalGuiObjects[id];
    }

    void destroy(){
      if(this->button != NULL){
        lv_obj_del(this->button);
        this->button = NULL;
      }
    }

    void setSize(int x, int y){
      lv_obj_set_size(this->button, LV_PCT(x), LV_PCT(y));
    }

    void setLabel(PalcomLabel label){
      this->label = label;
    }
    
    void setSimpleCallback(void(*func)(lv_event_t*)){
      lv_obj_add_event_cb(this->button, func, LV_EVENT_ALL, 0);
    }
    
    void setValuedCallback(void(*func)(lv_event_t*), uint8_t *val){
      lv_obj_add_event_cb(this->button, func, LV_EVENT_ALL, val);
    }
    void setRelativeAlignment(int ref, int x, int y){
      lv_obj_align_to(this->button, this->label.getObj(), ref, x,  y);
    }
};