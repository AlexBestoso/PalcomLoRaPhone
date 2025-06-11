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
#include "./tabview.h"

void PalcomTabview::setTabBarPosition(lv_dir_t dir){
	lv_tabview_set_tab_bar_position(this->getObject(), dir);
}

void PalcomTabview::create(lv_obj_t *parent){
	this->generate(parent, pal_tabview);
}

lv_obj_t *PalcomTabview::newTab(const char *name){
	return lv_tabview_add_tab(this->getObject(), name);
}

void PalcomTabview::renameTab(uint32_t idx, const char *new_name){
	lv_tabview_rename_tab(this->getObject(), idx, new_name);
}

void PalcomTabview::makeTabActive(uint32_t idx, bool animation){
	lv_tabview_set_active(this->getObject(), idx, (lv_anim_enable_t)animation);
}
void PalcomTabview::tabBarPositionNone(void){
	this->setTabBarPosition(LV_DIR_NONE);
}
void PalcomTabview::tabBarPositionLeft(void){
	this->setTabBarPosition(LV_DIR_LEFT);
}
void PalcomTabview::tabBarPositionRight(void){
	this->setTabBarPosition(LV_DIR_RIGHT);
}
void PalcomTabview::tabBarPositionTop(void){
	this->setTabBarPosition(LV_DIR_TOP);
}
void PalcomTabview::tabBarPositionBottom(void){
	this->setTabBarPosition(LV_DIR_BOTTOM);
}
void PalcomTabview::tabBarPositionHorizontal(void){
	this->setTabBarPosition(LV_DIR_HOR);
}
void PalcomTabview::tabBarPositionVertical(void){
	this->setTabBarPosition(LV_DIR_VER);
}
void PalcomTabview::tabBarPositionAll(void){
	this->setTabBarPosition(LV_DIR_ALL);
}


void PalcomTabview::setTabBarSize(int32_t size){
	lv_tabview_set_tab_bar_size(this->getObject(), size);
}

int PalcomTabview::tabCount(void){
	return lv_tabview_get_tab_count(this->getObject());
}

int PalcomTabview::getActiveTab(void){
	return lv_tabview_get_tab_active(this->getObject());
}

lv_obj_t *PalcomTabview::getContent(void){
	return lv_tabview_get_content(this->getObject());
}

lv_obj_t *PalcomTabview::getTabBar(void){
	return lv_tabview_get_tab_bar(this->getObject());
}
