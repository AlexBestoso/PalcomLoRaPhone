class PalcomTextarea{
  private:
    lv_obj_t *textarea = NULL;
    int objectId = -1;
  public:
    lv_obj_t *getObj(void){
      return textarea;
    }

    void create(lv_obj_t *parent){
      this->textarea = lv_textarea_create(parent);
    }

    void createGlobal(lv_obj_t *parent, uint id){
      if(id >= GLOBAL_GUI_OBJECT_COUNT){
        return;
      }
      objectId = id;
      globalGuiObjects[id] = lv_textarea_create(parent);
      this->textarea = globalGuiObjects[id];
    }

    void loadGlobal(int id){
      textarea = globalGuiObjects[id];
    }
    void destroy(){
      if(this->textarea != NULL){
        lv_obj_del(this->textarea);
        this->textarea = NULL;
      }
    }

    void setCursorClickPos(bool val){
      lv_textarea_set_cursor_click_pos(this->textarea, val);
    }

    void setTextSelection(bool val){
      lv_textarea_set_text_selection(this->textarea, val);
    }

    void setSize(int w, int h){
      lv_obj_set_size(this->textarea, w, h);
    }

    void setText(const char *text){
      lv_textarea_set_text(this->textarea, text);
    }

    void setMaxLength(int len){
      lv_textarea_set_max_length(this->textarea, len);
    }

    void setOneLine(bool val){
      lv_textarea_set_one_line(this->textarea, val);
    }

    void setAlignment(int ref, int x, int y){
      lv_obj_align(this->textarea, ref, x, y);
    }

    void setPasswordMode(bool val){
      lv_textarea_set_password_mode(this->textarea, val);
    }

    const char *getText(void){
      return lv_textarea_get_text(this->textarea);
    }
};