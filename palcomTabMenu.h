#define TAB_MENU_TAB_MAX 5
class PalcomTabMenu{
  private:
    lv_obj_t *menu = NULL;
    int objectId = -1;
    lv_obj_t *tabs[TAB_MENU_TAB_MAX] = {NULL};
  public:
    lv_obj_t *getObj(){
      return menu;
    }
    void create(lv_obj_t *parent){
      this->menu = lv_tabview_create(parent, LV_DIR_TOP, 50);
    }

    void createGlobal(lv_obj_t *parent, uint id){
      if(id >= GLOBAL_GUI_OBJECT_COUNT){
        return;
      }
      objectId = id;
      globalGuiObjects[id] = lv_tabview_create(parent, LV_DIR_TOP, 50);
      this->menu = globalGuiObjects[id];
    }

    void destroy(){
      for(int i=0; i<TAB_MENU_TAB_MAX; i++){
        if(this->tabs[i] != NULL){
          lv_obj_del(this->tabs[i]);
          this->tabs[i] = NULL;
        }
      }
      if(this->menu != NULL){
        lv_obj_del(this->menu);
        this->menu = NULL;
      }
    }

    void addTab(int id, const char *text){
      if(id >= TAB_MENU_TAB_MAX)
        return;
      tabs[id] = lv_tabview_add_tab(menu, text);
    }

    lv_obj_t *getTab(int id){
      if(id >= TAB_MENU_TAB_MAX)
        return tabs[0];
      return tabs[id];
    }

    void setActiveTab(int id, bool useAnimation){
      lv_anim_enable_t animation = (useAnimation) ? LV_ANIM_ON : LV_ANIM_OFF;
      lv_tabview_set_act(this->menu, id, animation);
    }

    int getActiveTab(){
      return lv_tabview_get_tab_act(this->menu);
    }
};