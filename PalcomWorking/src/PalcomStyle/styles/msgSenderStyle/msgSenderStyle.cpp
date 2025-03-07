#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>

#include "./msgSenderStyle.h"

MsgSenderStyle::MsgSenderStyle(void){
	this->initStyle4();
	this->initStyle3();
	this->initStyle2();
	this->initStyle();
}

MsgSenderStyle::~MsgSenderStyle(){}

lv_style_t *MsgSenderStyle::getStyle2(void){
	return &this->style2;
}

lv_style_t *MsgSenderStyle::getStyle3(void){
        return &this->style3;
}

lv_style_t *MsgSenderStyle::getStyle4(void){
	return &this->style4;
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

void MsgSenderStyle::initStyle3(void){
	this->setStyle(&this->style3);
        this->init();
	this->setShadowWidth(0);
	this->setShadowSpread(0);
	this->setShadowColor(this->makeColor(0xff, 0xff, 0xff));

	this->setBgOpacity(-1);
}

void MsgSenderStyle::initStyle4(void){
	this->setStyle(&this->style4);
        this->init();
	this->setShadowWidth(0);
        this->setShadowSpread(0);
	this->setShadowColor(this->makeColor(0xff, 0xff, 0xff));
	this->setBgOpacity(1);
}
