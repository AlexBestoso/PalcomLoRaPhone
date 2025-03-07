#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

class PalcomStyle;
#include "../../PalcomStyle/PalcomStyle.h"

#include "./ButtonStyle.h"

lv_color_t DebugScreenButtonStyle::darken(const lv_color_filter_dsc_t * dsc, lv_color_t color, lv_opa_t opa){
        LV_UNUSED(dsc);
	return lv_color_darken(color, opa);
}

lv_style_t *DebugScreenButtonStyle::getStyle(void){
	return (lv_style_t *)&submitButton;
}

lv_style_t *DebugScreenButtonStyle::getPressedStyle(void){
	return (lv_style_t *)&submitButtonPressed;
}

lv_color_filter_dsc_t *DebugScreenButtonStyle::getColorFilter(void){
	return (lv_color_filter_dsc_t *)&color_filter;
}
void DebugScreenButtonStyle::initStyle(){
	/*Create a simple button style*/
	this->setStyle(this->getStyle());
	this->init();
	this->setRadius(10);
	this->setBgOpacity(11);
	this->setBgColor(this->makeColor(0x10, 0x05, 0x10));
	this->setBgGradientColor(this->makeColor(0x80, 0x30, 0x80));
	this->setBgGradientDirection(2);
	this->setBorderColor(this->makeColor(0x55, 0, 0));
	this->setBorderOpacity(2);
	this->setBorderWidth(2);
	this->setTextColor(this->makeColor(0xff, 0xff, 0xff));

	/*Create a style for the pressed state.
   			*Use a color filter to simply modify all colors in this state*/
	this->setColorFilter(this->getColorFilter(), darken);
	this->initColorFilter();
	this->setStyle(this->getPressedStyle());
	this->init();
	this->applyColorFilter();
	this->setColorFilterOpacity(2);
	this->setBorderColor(this->makeColor(0x55, 0, 0));
	this->setBorderOpacity(2);
	this->setBorderWidth(12);
}


