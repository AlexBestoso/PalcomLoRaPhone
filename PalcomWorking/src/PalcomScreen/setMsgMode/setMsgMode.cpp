#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

#include <src/PalcomObject/PalcomObject.h>
#include <src/PalcomObject/Label/Label.h>
#include <src/PalcomObject/Button/Button.h>
#include <src/PalcomScreen/PalcomScreen.h>
#include <src/taskQueue/taskQueue.h>

extern int PalcomScreenError;
extern int palcome_message_mode;
extern TaskQueue taskQueue;
#include "./setMsgMode.h"

void SetMsgModeScreen::toggleMeshMode(lv_event_t *e){
	if(lv_event_get_code(e) == LV_EVENT_RELEASED){
		palcome_message_mode = PALMSG_MODE_MESH;
		PalcomScreenError = 1;
		taskQueue.push(taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GRAPHICS, GRAPHICS_INSTR_HOMEPAGE));
	}
}
void SetMsgModeScreen::toggleNodeMode(lv_event_t *e){
	if(lv_event_get_code(e) == LV_EVENT_RELEASED){
		palcome_message_mode = PALMSG_MODE_NODE;
		PalcomScreenError = 1;
		taskQueue.push(taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GRAPHICS, GRAPHICS_INSTR_HOMEPAGE));
	}
}
void SetMsgModeScreen::toggleUsbMode(lv_event_t *e){
	if(lv_event_get_code(e) == LV_EVENT_RELEASED){
		palcome_message_mode = PALMSG_MODE_USB;
		PalcomScreenError = 1;
		taskQueue.push(taskQueue.buildTask(TASK_SPACE_GRAPHICS, TASK_SPACE_GRAPHICS, GRAPHICS_INSTR_HOMEPAGE));
	}
}


SetMsgModeScreen::SetMsgModeScreen(void){
	this->buttonStyle.initStyle();
	PalcomScreenError = 0;
}

SetMsgModeScreen::~SetMsgModeScreen(){

}

void SetMsgModeScreen::reset(void){
	this->setBuildRequired(true);
	this->globalDestroy();
	this->destroy();
	this->clearScreenError();
}

void SetMsgModeScreen::generateObjects(void){
	lv_obj_t *screen = this->genScreen();
	this->setFullScreen();
	this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

	PalcomLabel title;
	title.create(screen);
	title.setLongMode(LV_LABEL_LONG_SCROLL);
	title.setWidth(300);
	/*if(palcome_message_mode == PALMSG_MODE_MESH)
		title.setText("Mesh Messaging");
	else if(palcome_message_mode == PALMSG_MODE_NODE)
		title.setText("Node Relay Messaging");
	else if(palcome_message_mode == PALMSG_MODE_USB)
		title.setText("USB Relay Messaging");
	title.setAlignment(LV_ALIGN_OUT_TOP_MID, 80, 20);
	*/

	int bWidth = 45;
	int bHeight = 45;

	PalcomButton button;
	button.create(screen);
	title.create(button.getObject());
	title.setText("Mesh Mode");
	title.center();
	button.setLabel(title);
	button.setDefaultStyle(this->buttonStyle.getStyle());	
	button.setPressedStyle(this->buttonStyle.getPressedStyle());
	button.setSize(bWidth, bHeight);
	button.setAlignment(LV_ALIGN_TOP_LEFT, 0, 0);
	button.setSimpleCallback(&toggleMeshMode);

	button.create(screen);
	title.create(button.getObject());
	title.setText("Node Mode");
	title.center();
	button.setLabel(title);
	button.setDefaultStyle(this->buttonStyle.getStyle());	
	button.setPressedStyle(this->buttonStyle.getPressedStyle());
	button.setSize(bWidth, bHeight);
	button.setAlignment(LV_ALIGN_BOTTOM_MID, 0, 0);
	button.setSimpleCallback(&toggleNodeMode);

	button.create(screen);
	title.create(button.getObject());
	title.setText("USB Mode");
	title.center();
	button.setLabel(title);
	button.setDefaultStyle(this->buttonStyle.getStyle());	
	button.setPressedStyle(this->buttonStyle.getPressedStyle());
	button.setSize(bWidth, bHeight);
	button.setAlignment(LV_ALIGN_TOP_RIGHT, 0, 0);
	button.setSimpleCallback(&toggleUsbMode);

}

int SetMsgModeScreen::run(void){
	if(PalcomScreenError == 1){
		this->reset();
		PalcomScreenError = 0;
	}
	this->buildCheck();
	this->execute();
	return 0;
}
