#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomEvent/PalcomEvent.h>
#include <src/PalcomObject/PalcomObject.h>

extern lv_obj_t *keyboardFocusedObj;
extern lv_group_t *keyboardGroup;

#include "./Textarea.h"

void PalcomTextarea::simpleCb(lv_event_t *e){
	PalcomEvent event(e);

	lv_obj_t *obj = (lv_obj_t*)event.getTarget();
	char *data = (char *)event.getUserData();
	
	if(event.getCode() == LV_EVENT_INSERT){
	}else if(event.getCode() == LV_EVENT_FOCUSED){
		keyboardFocusedObj = obj;
	}else if(event.getCode() == LV_EVENT_DEFOCUSED){
		keyboardFocusedObj = NULL;
	}
}

lv_obj_t *PalcomTextarea::getObj(void){
	return this->getObject();
}

void PalcomTextarea::pushChar(char c){
	lv_textarea_add_char(this->getObject(), c);
}

void PalcomTextarea::popCharLeft(void){
	lv_textarea_delete_char(this->getObject());
}
void PalcomTextarea::popCharRight(void){
	lv_textarea_delete_char_forward(this->getObject());
}

void PalcomTextarea::pushText(const char *t){
	lv_textarea_add_text(this->getObject(), t);
}

void PalcomTextarea::setText(const char *text){
	lv_textarea_set_text(this->getObject(), text);
}

void PalcomTextarea::setTextPlaceholder(const char *t){
	lv_textarea_set_placeholder_text(this->getObject(), t);
}

void PalcomTextarea::setStyle(lv_style_t *style, lv_style_t *focusedStyle){
	this->setDefaultStyle(style);
	this->setFocusedStyle(focusedStyle);
}

void PalcomTextarea::setStyleMain(lv_style_t *style){
	this->setAnyStyle(style, LV_PART_MAIN);
}

void PalcomTextarea::setStyleScrollbar(lv_style_t *style){
	this->setAnyStyle(style, LV_PART_SCROLLBAR);
}

void PalcomTextarea::setStyleSelected(lv_style_t *style){
	this->setAnyStyle(style, LV_PART_SELECTED);
}

void PalcomTextarea::setStyleCursor(lv_style_t *style){
	this->setAnyStyle(style, LV_PART_CURSOR);
}

void PalcomTextarea::setStylePlaceholder(lv_style_t *style){
	this->setAnyStyle(style, LV_PART_TEXTAREA_PLACEHOLDER);
}

void PalcomTextarea::create(lv_obj_t *parent){
	this->generate(parent, pal_textarea);
	this->setParamCallback(&simpleCb, this->getObject());
	lv_group_add_obj(keyboardGroup, this->getObject());
	//lv_obj_remove_style_all(this->getObject());
}

void PalcomTextarea::createGlobal(lv_obj_t *parent, uint id){
	this->generateGlobal(parent, id, pal_textarea);
}

void PalcomTextarea::setCursorClickPos(bool val){
	lv_textarea_set_cursor_click_pos(this->getObject(), val);
}

void PalcomTextarea::setTextSelection(bool val){
	lv_textarea_set_text_selection(this->getObject(), val);
}

void PalcomTextarea::setSize(int w, int h){
	lv_obj_set_size(this->getObject(), w, h);
}

void PalcomTextarea::setMaxLength(int len){
	lv_textarea_set_max_length(this->getObject(), len);
}

void PalcomTextarea::setOneLine(bool val){
	lv_textarea_set_one_line(this->getObject(), val);
}

void PalcomTextarea::setAlignment(lv_align_t ref, int x, int y){
	lv_obj_align(this->getObject(), ref, x, y);
}

void PalcomTextarea::setPasswordMode(bool val){
	lv_textarea_set_password_mode(this->getObject(), val);
}

const char *PalcomTextarea::getText(void){
	return lv_textarea_get_text(this->getObject());
}

size_t PalcomTextarea::getTextSize(void){
	if(this->getText() == NULL)
		return 0;
	return (size_t)strlen(this->getText());
}

void PalcomTextarea::moveCursor(int direction){
	switch(direction){
		case PALCOM_TA_CURSOR_UP:
			lv_textarea_cursor_up(this->getObject());
			break;
		case PALCOM_TA_CURSOR_DOWN:
			lv_textarea_cursor_down(this->getObject());
			break;
		case PALCOM_TA_CURSOR_LEFT:
			lv_textarea_cursor_left(this->getObject());
			break;
		case PALCOM_TA_CURSOR_RIGHT:
			lv_textarea_cursor_right(this->getObject());
			break;
	}
}

void PalcomTextarea::setValidChars(const char *s){
	lv_textarea_set_accepted_chars(this->getObject(), s);
}
