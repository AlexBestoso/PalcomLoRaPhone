#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

class PalcomObject;
#include "../PalcomObject.h"

#include "./Dropdown.h"

PalcomDropdown::PalcomDropdown(void){
	listOptions = "";
}

lv_obj_t *PalcomDropdown::getObj(void){
	return this->getObject();
}

void PalcomDropdown::create(lv_obj_t *parent){
	this->generate(parent, pal_dropdown);
}

void PalcomDropdown::setList(const char *opts){
	this->listOptions = opts;
	lv_dropdown_set_options(this->getObject(), opts);
}

const char *PalcomDropdown::getList(void){
	return this->listOptions.c_str();
}

void PalcomDropdown::getSelection(char *out, size_t size){
	if(out == NULL)
		return;
	lv_dropdown_get_selected_str(this->getObject(), out, size);
}

void PalcomDropdown::setSelection(int id){
	if(id < 0) return;
	lv_dropdown_set_selected(this->getObject(), id);
}
