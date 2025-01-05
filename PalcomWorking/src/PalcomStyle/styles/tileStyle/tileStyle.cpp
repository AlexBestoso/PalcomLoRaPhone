#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>
#include <src/PalcomStyle/PalcomStyle.h>
#include "tileStyle.h"

TileStyle::TileStyle(){
	this->initStyle();
}
TileStyle::~TileStyle(){

}

void TileStyle::initStyle(void){
	this->setStyle(&this->style);
	this->init();
	this->setBgOpacity(2);
}
