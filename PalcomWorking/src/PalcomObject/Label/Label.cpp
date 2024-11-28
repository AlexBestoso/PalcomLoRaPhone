#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

class PalcomObject;
#include "../PalcomObject.h"

#include "./Label.h"

lv_obj_t *PalcomLabel::getObj(void){
	return this->getObject();
}

void PalcomLabel::create(lv_obj_t *parent){
	this->generate(parent, pal_label);
}

void PalcomLabel::createGlobal(lv_obj_t *parent, uint id){
	this->generateGlobal(parent, id, pal_label);
}

/*
* Valid Modes : 
* LV_LABEL_LONG_EXPAND, LV_LABEL_LONG_BREAK, LV_LABEL_LONG_DOT,
* LV_LABEL_LONG_SCROLL, LV_LABEL_LONG_SCROLL_CIRC, LV_LABEL_LONG_CROP
*/
void PalcomLabel::setLongMode(lv_label_long_mode_t mode){
	lv_label_set_long_mode(this->getObject(), mode);
}

void PalcomLabel::setWidth(int width){
	lv_obj_set_width(this->getObject(), width);
}

void PalcomLabel::setText(const char *text){
	lv_label_set_text(this->getObject(), text);
}

