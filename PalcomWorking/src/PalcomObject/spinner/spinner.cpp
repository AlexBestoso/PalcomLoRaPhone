#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>

#include <TFT_eSPI.h>
#include <RadioLib.h>
#include <lvgl.h>
#include <string>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomObject/PalcomObject.h>

#include "./spinner.h"

void PalcomSpinner::create(lv_obj_t *parent){
	this->generate(parent, pal_spinner);
}

void PalcomSpinner::setAnimation(int time, int angle){
	lv_spinner_set_anim_params(this->getObject(), time, angle);
}
