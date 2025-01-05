#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>

#include "./msgSenderStyle.h"

MsgSenderStyle::MsgSenderStyle(void){
	this->initStyle2();
	this->initStyle();
}

MsgSenderStyle::~MsgSenderStyle(){}

lv_style_t *MsgSenderStyle::getStyle2(void){
	return &this->style2;
}

void MsgSenderStyle::initStyle(void){
	this->setStyle(&this->style);
        this->init();
	this->setShadowWidth(10);
	this->setShadowSpread(5);
	this->setShadowColor(this->makeColor(0xff, 0xff, 0xff));

	this->setBgOpacity(8);
}

void MsgSenderStyle::initStyle2(void){
	this->setStyle(&this->style2);
        this->init();
	this->setShadowWidth(0);
	this->setShadowSpread(0);
	this->setShadowColor(this->makeColor(0xff, 0xff, 0xff));

	this->setBgOpacity(8);
}


