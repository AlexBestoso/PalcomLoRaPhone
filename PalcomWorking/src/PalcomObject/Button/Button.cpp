#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include "../PalcomObject.h"
class PalcomObject;
#include "../Label/Label.h"

#include "./Button.h"

lv_obj_t *PalcomButton::getObj(){
	return this->getObject();
}

void PalcomButton::create(lv_obj_t *parent){
	this->generate(parent, pal_button);
}

void PalcomButton::createGlobal(lv_obj_t *parent, int id){
	this->generateGlobal(parent, id, pal_button);
}

void PalcomButton::setStyle(lv_style_t *style, lv_style_t *pressedStyle){
	this->setDefaultStyle(style);
	this->setPressedStyle(pressedStyle);
}

lv_style_t PalcomButton::getStyle(void){
	return this->style;
}

void PalcomButton::setLabel(PalcomLabel label){
	this->label = label;
}

void PalcomButton::setSimpleCallback(void(*func)(lv_event_t*)){
	lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_ALL, 0);
}

void PalcomButton::setValuedCallback(void(*func)(lv_event_t*), uint8_t *val){
	lv_obj_add_event_cb(this->getObject(), func, LV_EVENT_ALL, val);
}

void PalcomButton::setRelativeAlignment(lv_align_t ref, int x, int y){
	lv_obj_align_to(this->getObject(), this->label.getObject(), ref, x,  y);
}
