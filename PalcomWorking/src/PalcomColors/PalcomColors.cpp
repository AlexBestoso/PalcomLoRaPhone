#include <lvgl.h>
#include <Arduino.h>
#include <cstdint>
#include "./PalcomColors.h"
int PalcomColors::getColorBrightness(lv_color_t c){ // depreicated
	return lv_color_brightness(c);
}
int PalcomColors::getBrightness(lv_color_t c){
	return lv_color_brightness(c);
}

lv_color_t PalcomColors::darken(lv_color_t c, int adjuster){
	return lv_color_darken(c, adjuster);
}

lv_color_t PalcomColors::make(int r, int g, int b){
	return LV_COLOR_MAKE(r, g, b);
}
