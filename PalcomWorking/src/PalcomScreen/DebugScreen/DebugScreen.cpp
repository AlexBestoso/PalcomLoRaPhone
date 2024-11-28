#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include "../../PalcomObject/PalcomObject.h"
#include "../../PalcomObject/Label/Label.h"
#include "../../PalcomObject/Button/Button.h"
#include "../../PalcomStyle/PalcomStyle.h"
#include "../../PalcomScreen/PalcomScreen.h"

extern int PalcomScreenError;
#include "./DebugScreen.h"

void PalcomDebugScreen::buttonCallback(lv_event_t *e){
	if(lv_event_get_code(e) == LV_EVENT_RELEASED)
		Serial.printf("Button Released.\n");
}

PalcomDebugScreen::PalcomDebugScreen(void){
	this->buttonStyle.initStyle();
}

PalcomDebugScreen::~PalcomDebugScreen(){

}

void PalcomDebugScreen::reset(void){
	this->setBuildRequired(true);
	this->globalDestroy();
	this->destroy();
	this->clearScreenError();
}

void PalcomDebugScreen::generateObjects(void){
	lv_obj_t *screen = this->genScreen();
	this->setFullScreen();
	this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

	PalcomLabel title;
	title.create(screen);
	title.setLongMode(LV_LABEL_LONG_SCROLL);
	title.setWidth(300);
	title.setText("Debug Screen");
	title.setAlignment(LV_ALIGN_OUT_TOP_MID, 80, 20);

	PalcomButton button;
	button.create(screen);
	title.create(button.getObject());
	title.setText("LoRa");
	title.center();
	button.setLabel(title);
	button.setDefaultStyle(this->buttonStyle.getStyle());	
	//button.setPressedStyle(this->buttonStyle.getPressedStyle());
	button.setSize(50, 15);
	button.setAlignment(LV_ALIGN_OUT_TOP_RIGHT, 80, 190);
	button.setSimpleCallback(&buttonCallback);
}

int PalcomDebugScreen::run(void){
	this->buildCheck();
	this->execute();
	return 0;
}
