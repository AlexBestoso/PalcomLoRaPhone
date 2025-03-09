#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>


#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>
#include "./PalcomObject.h"
extern lv_obj_t *globalGuiObjects[GLOBAL_GUI_OBJECT_COUNT];
// Private
// Public
		
PalcomObject::PalcomObject(void) : PalcomStyle(){

}
PalcomObject::PalcomObject(bool useBaseStyle) : PalcomStyle(useBaseStyle){
	this->init();

}
PalcomObject::PalcomObject(lv_style_t *baseStyle) : PalcomStyle(baseStyle){
	this->init();
}

lv_obj_t* PalcomObject::getObject(void){
	return this->object;
}
		
void PalcomObject::setObject(lv_obj_t *object){
	this->object = object;
}
		
int PalcomObject::getObjectId(void){
	return id;
}
		
void PalcomObject::setObjectId(int id){
	this->id = id;
}

		
void PalcomObject::destroy(void){
	if(this->object != NULL){
		lv_obj_del(this->object);
		this->object = NULL;
	}
}
	
void PalcomObject::create(lv_obj_t *parent){
	this->generate(parent, pal_label);
}
                
void PalcomObject::createGlobal(lv_obj_t *parent, uint id){
	this->generateGlobal(parent, id, pal_label);
}

		
void PalcomObject::addState(lv_state_t s){
	lv_obj_add_state(this->object, s);
}

		
void PalcomObject::removeState(lv_state_t s){
	lv_obj_clear_state(this->object, s);
}

		
bool PalcomObject::stateInUse(lv_state_t s){
	return lv_obj_has_state(this->object, s);
}

		
void PalcomObject::generate(lv_obj_t *parent, PalcomObjectType objtype, const char *data){
	switch(objtype){
	case pal_menupage:
		this->object = lv_menu_page_create(parent, data);
		break;
	defeault:
		this->generate(parent, objtype);
	}
}

		
void PalcomObject::generate(lv_obj_t *parent, PalcomObjectType objtype){	
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
		case pal_image:
			this->object = lv_image_create(parent);
			break;
		case pal_menu:
			this->object = lv_menu_create(parent);
			break;
		case pal_msgbox:
//			this->object = lv_msgbox_create(parent);
			break;
		case pal_checkbox:
			this->object = lv_checkbox_create(parent);
			break;
		case pal_menusect:
			this->object = lv_menu_section_create(parent);
			break;
		case pal_menupage:
			this->object = lv_menu_page_create(parent, NULL);
			break;
		case pal_slider:
			this->object = lv_slider_create(parent);
			break;
		case pal_switch:
			this->object = lv_switch_create(parent);
			break;
		case pal_tileview:
			this->object = lv_tileview_create(parent);
			break;
		case pal_line:
			this->object = lv_line_create(parent);
			break;
		case pal_triangle:
			this->object = lv_obj_create(parent);
			break;
	}
}

/*
 * Depreciated Concept
 * */	
void PalcomObject::generateGlobal(lv_obj_t *parent, uint id, PalcomObjectType objtype){
                        
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

		
void PalcomObject::loadGlobal(int id){
	this->object = globalGuiObjects[id];
}


void PalcomObject::setAnyStyle(lv_style_t *s, lv_style_selector_t sel){
	lv_obj_add_style(this->object, s, sel);
}


void PalcomObject::setDefaultStyle(lv_style_t *s){
	if(s == NULL){
		Serial.printf("NULL STYLE DETECTED\n");
		return;
	}
	lv_obj_add_style(this->object, s, LV_STATE_DEFAULT);
}


void PalcomObject::setCheckedStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_CHECKED);
}


void PalcomObject::setFocusedStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_FOCUSED);
}


void PalcomObject::setFocusedKeyStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_FOCUS_KEY);
}


void PalcomObject::setEditedStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_EDITED);
}


void PalcomObject::setHoveredStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_HOVERED);
}


void PalcomObject::setPressedStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_PRESSED);
}


void PalcomObject::setScrolledStyle(lv_style_t *s){
	lv_obj_add_style(this->object, s, LV_STATE_SCROLLED);
}


void PalcomObject::setDisabledStyle(lv_style_t *s){
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

void PalcomObject::setAlignment(lv_align_t ref, int x, int y){
	lv_obj_align(this->object, ref, x, y);
}


void PalcomObject::setSize(int x, int y){
	lv_obj_set_size(this->object, LV_PCT(x), LV_PCT(y));
}


void PalcomObject::setSizeRaw(int x, int y){
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

void PalcomObject::setScreenScrollDirection(lv_dir_t direction){
	lv_obj_set_scroll_dir(this->object, direction);
}

/*
* Scrollbars are displayed according to a configured mode. The following mode(s) exist:
* LV_SCROLLBAR_MODE_OFF: Never show the scrollbars
* LV_SCROLLBAR_MODE_ON: Always show the scrollbars
* LV_SCROLLBAR_MODE_ACTIVE: Show scroll bars while an object is being scrolled
* LV_SCROLLBAR_MODE_AUTO: Show scroll bars when the content is large enough to be scrolled
*/
void PalcomObject::setScrollMode(lv_scrollbar_mode_t mode){
	lv_obj_set_scrollbar_mode(this->object, mode);
}

void PalcomObject::center(void){
	lv_obj_center(this->object);
}

int PalcomObject::getHorizontalResolution(void){
	return lv_disp_get_hor_res(NULL);
}

int PalcomObject::getVirticalResolution(void){
	return lv_disp_get_ver_res(NULL);
}

void PalcomObject::fullScreen(void){
	lv_obj_set_size(this->object, this->getHorizontalResolution(), this->getVirticalResolution());
}

void PalcomObject::execute(){
	lv_timer_handler();
}


/*
 * Draw event callbacks
 * */

// LV_EVENT_COVER_CHECK: Check if Widget fully covers an area. The event parameter is lv_cover_check_info_t *.
void PalcomObject::setDrawCoverCheckCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_COVER_CHECK, input);
}
    //LV_EVENT_REFR_EXT_DRAW_SIZE: Get the required extra draw area around Widget (e.g. for shadow). The event parameter is int32_t * to store the size.
void PalcomObject::setDrawExtSizeCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_REFR_EXT_DRAW_SIZE, input);
}
    //LV_EVENT_DRAW_MAIN_BEGIN: Starting the main drawing phase
void PalcomObject::setDrawMainBeginCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_MAIN_BEGIN, input);
}
    //LV_EVENT_DRAW_MAIN: Perform the main drawing
void PalcomObject::setDrawMainCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_MAIN, input);
}

    //LV_EVENT_DRAW_POST_BEGIN: Starting the post draw phase (when all children are drawn)
void PalcomObject::setDrawPostBeginCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_POST_BEGIN, input);
}
    //LV_EVENT_DRAW_POST: Perform the post draw phase (when all children are drawn)
void PalcomObject::setDrawPostCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_POST, input);
}
    //LV_EVENT_DRAW_POST_END: Finishing the post draw phase (when all children are drawn)
void PalcomObject::setDrawPostEndCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_POST_END, input);
}
    //LV_EVENT_DRAW_TASK_ADDED: Adding a draw task
void PalcomObject::setDrawTaskAddCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_TASK_ADDED, input);
}

//LV_EVENT_DRAW_MAIN_END: Finishing the main drawing phase
void PalcomObject::setDrawMainEndCallback(void(*func)(lv_event_t*), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_DRAW_MAIN_END, input);
}

void PalcomObject::setSimpleCallback(void(*func)(lv_event_t*)){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_ALL, 0);
}

void PalcomObject::setParamCallback(void(*func)(lv_event_t *), void *input, lv_event_code_t event){
	lv_obj_add_event_cb(this->object, func, event, input);
}
void PalcomObject::setParamCallback(void(*func)(lv_event_t *), void *input){
	lv_obj_add_event_cb(this->object, func, LV_EVENT_ALL, input);
}

void PalcomObject::addFlag(lv_obj_flag_t f){
	setFlag(f);
}

void PalcomObject::setFlag(lv_obj_flag_t f){
	lv_obj_add_flag(this->object, f);
}

void PalcomObject::unsetFlag(lv_obj_flag_t f){
	lv_obj_clear_flag(this->object, f);
}

lv_color_t PalcomObject::getStyleBgColor(int v){
	return lv_obj_get_style_bg_color(this->object, v);
}
lv_color_t PalcomObject::getStyleBgColor(void){
	return lv_obj_get_style_bg_color(this->object, 0);
}

void PalcomObject::setStyleBgColor(lv_color_t c, int v){
	lv_obj_set_style_bg_color(this->object, c, v);
}
void PalcomObject::setStyleBgColor(lv_color_t c){
	lv_obj_set_style_bg_color(this->object, c, 0);
}

void PalcomObject::setStyleBgOpacity(int o){
	switch(o){
                case 0:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_0, 0);
                        break;
                case 1:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_10, 0);
                        break;
                case 2:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_20, 0);
                        break;
                case 3:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_30, 0);
                        break;
                case 4:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_40, 0);
                        break;
                case 5:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_50, 0);
                        break;
                case 6:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_60, 0);
                        break;
                case 7:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_70, 0);
                        break;
                case 8:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_80, 0);
                        break;
                case 9:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_90, 0);
                        break;
                case 10:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_100, 0);
                        break;
                case -1:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_TRANSP, 0);
                        break;
                default:
                        lv_obj_set_style_bg_opa(this->object, LV_OPA_COVER, 0);
                        break;
        }
}

lv_opa_t PalcomObject::getStyleBgOpacity(void){
	return lv_obj_get_style_bg_opa(this->object, 0);
}

lv_opa_t PalcomObject::getStyleBorderOpacity(void){
	return lv_obj_get_style_border_opa(this->object, 0);
}
void PalcomObject::setStyleBorderOpacity(int o){
	switch(o){
                case 0:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_0, 0);
                        break;
                case 1:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_10, 0);
                        break;
                case 2:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_20, 0);
                        break;
                case 3:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_30, 0);
                        break;
                case 4:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_40, 0);
                        break;
                case 5:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_50, 0);
                        break;
                case 6:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_60, 0);
                        break;
                case 7:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_70, 0);
                        break;
                case 8:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_80, 0);
                        break;
                case 9:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_90, 0);
                        break;
                case 10:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_100, 0);
                        break;
                case -1:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_TRANSP, 0);
                        break;
                default:
                        lv_obj_set_style_border_opa(this->object, LV_OPA_COVER, 0);
                        break;
        }

}


void PalcomObject::setStyleRotation(int r){
	lv_obj_set_style_transform_rotation(this->object, r, 0);
}

void PalcomObject::setStylePaddingHor(int padding, lv_style_selector_t sel){
	lv_obj_set_style_pad_hor(this->object, padding, sel);	
}

void PalcomObject::setStylePaddingHor(int padding){
	lv_obj_set_style_pad_hor(this->object, padding, 0);	
}

int PalcomObject::getStylePaddingLeft(lv_part_t p){
	return lv_obj_get_style_pad_left((const lv_obj_t *)this->getObject(), p);
}

int PalcomObject::getStylePaddingLeft(void){
	return lv_obj_get_style_pad_left((const lv_obj_t *)this->getObject(), LV_PART_MAIN);
}

void PalcomObject::setFlexGrow(int v){
	lv_obj_set_flex_grow(this->getObject(), v);
}

int32_t PalcomObject::getHeight(void){
	//return lv_obj_get_height(this->getObject());
	return lv_obj_get_self_height(this->getObject());
}
void PalcomObject::setHeight(uint32_t height){
	lv_obj_set_height(this->getObject(), height);
}
