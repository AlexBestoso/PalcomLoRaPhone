enum {
    LV_MENU_ITEM_BUILDER_VARIANT_1,
    LV_MENU_ITEM_BUILDER_VARIANT_2
};
typedef uint8_t lv_menu_builder_variant_t;

//static void back_event_handler(lv_event_t * e);
//static void switch_handler(lv_event_t * e);
//lv_obj_t * root_page;

/*static void switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = (lv_obj_t *)lv_event_get_user_data(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, root_page);
            lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED, NULL);
        }
        else {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
    /*        lv_menu_set_page(menu, root_page);
        }
    }
}*/


bool settingsMenu_exit = false;
class SettingsMenu : public PalcomMenu{
	private:
		PalcomPage generalPage;
		PalcomPage securityPage;
		PalcomPage relayPage;
		PalcomPage resetPage;

		PalcomButton backBtn;
		PalcomObject rootPg;

		static void callback_exitSettingsMenu(lv_event_t * e){
    			lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    			lv_obj_t * menu = (lv_obj_t*)lv_event_get_user_data(e);

    			if(lv_menu_back_button_is_root(menu, obj) && lv_event_get_code(e) == LV_EVENT_RELEASED) {
				settingsMenu_exit = true;
    			}
		}

		lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt, lv_menu_builder_variant_t builder_variant){
    			lv_obj_t * obj = this->createContainer(parent);
		
    			lv_obj_t * img = NULL;
    			PalcomLabel label;

    			if(icon) {
        			img = lv_img_create(obj);
        			lv_img_set_src(img, icon);
    			}

    			if(txt) {
        			label.create(obj);
        			label.setText(txt);
        			label.setLongMode(LV_LABEL_LONG_SCROLL_CIRCULAR);
        			label.setFlexGrow(1);
    			}

    			if(builder_variant == LV_MENU_ITEM_BUILDER_VARIANT_2 && icon && txt) {
        			lv_obj_add_flag(img, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
        			lv_obj_swap(img, label.getObject());
    			}

    			return obj;
		}

		lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max, int32_t val){
    			lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_2);

    			lv_obj_t * slider = lv_slider_create(obj);
    			lv_obj_set_flex_grow(slider, 1);
    			lv_slider_set_range(slider, min, max);
    			lv_slider_set_value(slider, val, LV_ANIM_OFF);

    			if(icon == NULL) {
        			lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    			}

    			return obj;
		}

		lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk){
    			lv_obj_t * obj = create_text(parent, icon, txt, LV_MENU_ITEM_BUILDER_VARIANT_1);

    			lv_obj_t * sw = lv_switch_create(obj);
    			lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    			return obj;
		}

		static void simpleCb(lv_event_t *e){
			lv_obj_t *obj = (lv_obj_t*)lv_event_get_target(e);
			char *data = (char *)lv_event_get_user_data(e);

			if(lv_event_get_code(e) == LV_EVENT_INSERT){
			}else if(lv_event_get_code(e) == LV_EVENT_FOCUSED){
				keyboardFocusedObj = obj;
			}else if(lv_event_get_code(e) == LV_EVENT_DEFOCUSED){
				keyboardFocusedObj == NULL;
			}


		}

		lv_obj_t *create_textarea(lv_obj_t *parent, const char *icon, const char *text){
			lv_obj_t *obj = this->create_text(parent, icon, text, LV_MENU_ITEM_BUILDER_VARIANT_2);
			PalcomTextarea textarea;
			textarea.create(obj);
			textarea.setSize(120, 40);
			

			return obj;

		}

	public:
		void make(lv_obj_t *parent, void (*exitFunc)(lv_event_t *)){
			this->create(parent);

	    		lv_color_t bg_color = this->getStyleBgColor();
	    		if(this->colors.getBrightness(bg_color) > 127) {
		    		this->setStyleBgColor(this->colors.darken(this->getStyleBgColor(), 10));
	    		}else {
		    		this->setStyleBgColor(this->colors.darken(this->getStyleBgColor(), 50));
	    		}

	    		this->enableRootBackButton(true);
			this->setParamCallback(exitFunc, (void*)this->getObject(), LV_EVENT_CLICKED);
			this->fullScreen();
	    		this->center();

		    	lv_obj_t * cont;
			PalcomSection section;

			/* Create General Page */
	    		generalPage.create(this->getObject(), NULL);
	    		generalPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
			generalPage.addSeparator();
	    		generalPage.section.create(generalPage.getObject());
	    		this->create_textarea(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "Name");
	    		this->create_slider(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "LockTimer", 0, 150, 50);
	    		this->create_slider(generalPage.section.getObject(), LV_SYMBOL_SETTINGS, "Brightness", 0, 150, 50);

			/* Create Security Page */
			securityPage.create(this->getObject(), NULL);
	    		securityPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
	    		securityPage.addSeparator();
	    		securityPage.section.create(securityPage.getObject());
	    		this->create_switch(securityPage.section.getObject(), LV_SYMBOL_AUDIO, "PMode", false);
	    		this->create_switch(securityPage.section.getObject(), LV_SYMBOL_AUDIO, "Change Passcode", false);
			this->

			/* Create Relay Page */
			relayPage.create(this->getObject(), NULL);
	    		relayPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
	    		relayPage.addSeparator();
	    		relayPage.section.create(relayPage.getObject());
	    		this->create_slider(relayPage.section.getObject(), LV_SYMBOL_SETTINGS, "Relay IP", 0, 150, 100);
	    		this->create_slider(relayPage.section.getObject(), LV_SYMBOL_SETTINGS, "Relay Port", 0, 150, 100);

			/* Create factory reset page */
			resetPage.create(this->getObject(), NULL);
	    		resetPage.setStylePaddingHor(this->getMainHeaderPaddingLeft());
	    		resetPage.addSeparator();
	    		resetPage.section.create(resetPage.getObject());
			// Add button here.

	    		/*Create a root page*/
			this->page.create(this->getObject(), "Settings");
			this->page.setStylePaddingHor(this->getMainHeaderPaddingLeft());
	    		
			this->page.section.create(this->page.getObject());
	    		cont = this->create_text(this->page.section.getObject(), LV_SYMBOL_SETTINGS, "General", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, generalPage.getObject());
	    		
			cont = this->create_text(this->page.section.getObject(), LV_SYMBOL_SETTINGS, "Security", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, securityPage.getObject());
	    		
			cont = this->create_text(this->page.section.getObject(), LV_SYMBOL_SETTINGS, "Relay", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, relayPage.getObject());

	    		this->create_text(this->page.getObject(), NULL, "Admin", LV_MENU_ITEM_BUILDER_VARIANT_1);
	    		
			this->page.section.create(this->page.getObject());
    			cont = this->create_text(this->page.section.getObject(), NULL, "Factory Reset", LV_MENU_ITEM_BUILDER_VARIANT_1);
			this->linkContainerToPage(cont, resetPage.getObject());

    			this->setSidebarPage(this->page.getObject());
	
    			lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(this->getObject()), 0), 0), LV_EVENT_CLICKED,NULL);
		}
};
