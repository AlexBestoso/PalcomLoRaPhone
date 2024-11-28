#include <lvgl.h>
#include <Arduino.h>
#include <cstdint>

#include "../error/error.h"
#include "../PalcomObject/PalcomObject.h"
#if !defined GLOBAL_GUI_OBJECT_COUNT
	#define GLOBAL_GUI_OBJECT_COUNT 1
#endif
extern lv_obj_t *globalGuiObjects[GLOBAL_GUI_OBJECT_COUNT];


#include "./PalcomScreen.h"
extern int PalcomScreenError;

PalcomScreen::PalcomScreen(void){
	this->clearScreenError();
	this->screen = NULL;
	this->bgImage = NULL;
    	this->objectId = -1;
	this->buildRequired = true;

}
PalcomScreen::~PalcomScreen(){}

void PalcomScreen::clearScreenError(void){
	PalcomScreenError = 0;
}

bool PalcomScreen::isScreenError(void){
	return (PalcomScreenError == 0) ? false : true;
}

void PalcomScreen::setScreenError(int e){
	PalcomScreenError = e;
}

int PalcomScreen::getScreenError(){
	int ret = PalcomScreenError;
	PalcomScreenError = 0;
	return ret;
}
bool PalcomScreen::getBuildRequired(void){
	return buildRequired;
}
void PalcomScreen::setBuildRequired(bool v){
	buildRequired = v;
}

void PalcomScreen::create(void){
	this->screen = lv_screen_active();
	if(this->screen == NULL){
		throw CoreException("PalcomScreen::create() - Failed to fetch active screen.", 0x01);
	}
	object.setObject(this->screen);
}

void PalcomScreen::createGlobal(void){
	lv_obj_clean(lv_screen_active());
	globalGuiObjects[0] = lv_screen_active();
	objectId = 0;
	screen = globalGuiObjects[0];
	object.setObject(this->screen);
}

void PalcomScreen::destroy(void){
	lv_obj_clean(lv_screen_active());
	this->screen = NULL;
	this->bgImage = NULL;
}

void PalcomScreen::globalDestroy(void){
	for(int i=0; i<GLOBAL_GUI_OBJECT_COUNT; i++){
		if(globalGuiObjects[i] != NULL){
			lv_obj_del(globalGuiObjects[i]);
			globalGuiObjects[i] = NULL;
		}
	}
}

void PalcomScreen::load(void){
	generateObjects();
}

lv_obj_t *PalcomScreen::getScreen(void){
	return screen;
}

void PalcomScreen::setFullScreen(void){
	if(this->screen == NULL){
		while(1){Serial.printf("FUCKL\n");delay(1000);}
	}
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
void PalcomScreen::setScreenScrollDirection(lv_dir_t direction){
	lv_obj_set_scroll_dir(this->screen, direction);
}
/*
* Scrollbars are displayed according to a configured mode. The following mode(s) exist:
* LV_SCROLLBAR_MODE_OFF: Never show the scrollbars
* LV_SCROLLBAR_MODE_ON: Always show the scrollbars
* LV_SCROLLBAR_MODE_ACTIVE: Show scroll bars while an object is being scrolled
* LV_SCROLLBAR_MODE_AUTO: Show scroll bars when the content is large enough to be scrolled
*/
void PalcomScreen::setScrollMode(lv_scrollbar_mode_t mode){
	lv_obj_set_scrollbar_mode(this->screen, mode);
}

void PalcomScreen::setBgImage(const lv_img_dsc_t *img[1]){
	if(this->bgImage==NULL)
		this->bgImage = lv_img_create(this->screen);
	lv_img_set_src(this->bgImage, (void *)(img[0]));
}

void PalcomScreen::setBgX(int offset){
	if(this->bgImage==NULL)
		this->bgImage = lv_img_create(this->screen);
	lv_img_set_offset_x(this->bgImage, offset);
}

void PalcomScreen::setBgY(int offset){
	if(this->bgImage==NULL)
		this->bgImage = lv_img_create(this->screen);
	lv_img_set_offset_y(this->bgImage, offset);
}

void PalcomScreen::execute(void){
	lv_tick_inc(lv_timer_handler());
}
void PalcomScreen::e(void){
	execute();
}

void PalcomScreen::generateObjects(void){

}

void PalcomScreen::setFlag(lv_obj_flag_t f){
	lv_obj_add_flag(this->screen, f);
}

void PalcomScreen::unsetFlag(lv_obj_flag_t f){
	lv_obj_clear_flag(this->screen, f);
}

bool PalcomScreen::buildCheck(void){
	if(this->getBuildRequired()){
		this->setBuildRequired(false);
		this->load();
		return true;
	}
	return false;
}

lv_obj_t *PalcomScreen::genScreen(void){
	lv_obj_t *screen = this->getScreen();
	if(screen == NULL){
		this->globalDestroy();
		this->create();
		screen = this->getScreen();
	}
	return screen;
}
