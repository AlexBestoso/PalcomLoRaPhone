#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>
#include <defines.h>


#include <src/error/error.h>

#include <src/PalcomEvent/PalcomEvent.h>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

#include <src/PalcomObject/PalcomObject.h>
#include <src/PalcomObject/Label/Label.h>
#include <src/PalcomObject/Button/Button.h>
#include <src/PalcomObject/Textarea/Textarea.h>
#include <src/PalcomObject/Tileview/Tileview.h>
#include <src/PalcomObject/Image/Image.h>
#include <src/PalcomObject/Line/Line.h>
#include <src/PalcomObject/Triangle/Triangle.h>

#include <src/PalcomScreen/PalcomScreen.h>
#include <src/taskQueue/taskQueue.h>

#define USER_BUF_SIZE 256

extern const lv_image_dsc_t spaceAI2;
extern int PalcomScreenError;
extern int palcome_message_mode;
extern TaskQueue taskQueue;
extern uint8_t userBuffer[USER_BUF_SIZE];
extern size_t userBufferSize;
extern lv_obj_t *keyboardFocusedObj;

#include "./DebugScreen.h"

void PalcomDebugScreen::sendMessage(lv_event_t *e){
	try{
		PalcomEvent event(e);
		if(keyboardFocusedObj == NULL){
			return;
		}

		PalcomTextarea textarea;
		textarea.setObject(keyboardFocusedObj);
		if(strlen(textarea.getText()) <= 0)
			return;
		
		struct task_queue_task t;
		t.active = true;
		t.to = TASK_SPACE_COMMS;
		t.from = TASK_SPACE_GRAPHICS;
		t.instruction = COMMS_INSTR_SEND;
		const char *msg = textarea.getText();
		for(int i=0; i<256 && i<textarea.getTextSize(); i++){
			t.msg[i] = msg[i];
		}
		taskQueue.push(&t);
	
		textarea.setText("");
	}catch(CoreException e){
		e.out();
	}
}

void PalcomDebugScreen::toggleMeshMode(lv_event_t *e){
	PalcomEvent event(e);
	if(event.getCode() == LV_EVENT_VALUE_CHANGED)
		palcome_message_mode = 0;
}

void PalcomDebugScreen::toggleNodeMode(lv_event_t *e){
	PalcomEvent event(e);
	if(event.getCode() == LV_EVENT_VALUE_CHANGED)
		palcome_message_mode = 1;
}

void PalcomDebugScreen::toggleUsbMode(lv_event_t *e){
	PalcomEvent event(e);
	if(event.getCode() == LV_EVENT_VALUE_CHANGED)
		palcome_message_mode = 2;
}
PalcomDebugScreen::PalcomDebugScreen(void){
	this->buttonStyle.initStyle();
	PalcomScreenError = 0;
}

PalcomDebugScreen::~PalcomDebugScreen(){

}

void PalcomDebugScreen::buildModeSelect(lv_obj_t *target){
	PalcomObject background;

	background.generate(target, pal_base);
        background.setDefaultStyle(this->msgSenderStyle.getStyle2());
        background.setSize(100, 100);
        background.setAlignment(LV_ALIGN_TOP_LEFT, -1, -1);
        background.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        background.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	
	PalcomLabel title;

        int bWidth = 45;
        int bHeight = 45;

        meshButton.create(background.getObject());
        title.create(meshButton.getObject());
        title.setText("Mesh Mode");
        title.center();
        meshButton.setLabel(title);
        meshButton.setDefaultStyle(this->buttonStyle.getStyle());
        meshButton.setPressedStyle(this->buttonStyle.getPressedStyle());
        meshButton.setSize(bWidth, bHeight);
        meshButton.setAlignment(LV_ALIGN_TOP_LEFT, 0, 0);
        meshButton.setSimpleCallback(&this->toggleMeshMode);
	if(palcome_message_mode == 0){
                meshButton.addState(LV_STATE_CHECKED);
		meshButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
        }else{
                meshButton.removeState(LV_STATE_CHECKED);
		meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
	}

        nodeButton.create(background.getObject());
        title.create(nodeButton.getObject());
        title.setText("Node Mode");
        title.center();
        nodeButton.setLabel(title);
        nodeButton.setDefaultStyle(this->buttonStyle.getStyle());
        nodeButton.setPressedStyle(this->buttonStyle.getPressedStyle());
        nodeButton.setSize(bWidth, bHeight);
        nodeButton.setAlignment(LV_ALIGN_BOTTOM_MID, 0, 0);
        nodeButton.setSimpleCallback(&this->toggleNodeMode);
	if(palcome_message_mode == 1){
                nodeButton.addState(LV_STATE_CHECKED);
		nodeButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
        }else{
                nodeButton.removeState(LV_STATE_CHECKED);
		nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
	}

        usbButton.create(background.getObject());
        title.create(usbButton.getObject());
        title.setText("USB Mode");
        title.center();
        usbButton.setLabel(title);
        usbButton.setDefaultStyle(this->buttonStyle.getStyle());
        usbButton.setPressedStyle(this->buttonStyle.getPressedStyle());
        usbButton.setSize(bWidth, bHeight);
        usbButton.setAlignment(LV_ALIGN_TOP_RIGHT, 0, 0);
        usbButton.setSimpleCallback(&this->toggleUsbMode);
	if(palcome_message_mode == 2){
                usbButton.addState(LV_STATE_CHECKED);
		usbButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
        }else{
                usbButton.removeState(LV_STATE_CHECKED);
		usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
	}
	/*int offsetX = 0;
	int offsetY = 0;
	meshButton.create(background.getObject());
        //meshButton.setDefaultStyle(this->msgSenderStyle.getStyle4());
        
	meshButton.setSizeRaw(SCREEN_HOR, SCREEN_VIR);
	meshButton.setAlignment(LV_ALIGN_CENTER, 0, 0);
	meshButton.setStyleRotation(0);	

	meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
        meshButton.setSimpleCallback(&this->toggleMeshMode);

	if(palcome_message_mode == 0)
		meshButton.addState(LV_STATE_CHECKED);
	else
		meshButton.removeState(LV_STATE_CHECKED);

	int nodeAdjust = 0;
	nodeButton.create(background.getObject());
        
	nodeButton.setSize(RATIO_X*24, RATIO_Y*24);
	//nodeButton.setStyleRotation(450);
	nodeButton.setAlignment(LV_ALIGN_CENTER, RATIO_X*0, RATIO_Y*0);
	
	nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
        nodeButton.setSimpleCallback(&this->toggleNodeMode);
	if(palcome_message_mode == 1)
		nodeButton.addState(LV_STATE_CHECKED);
	else
		nodeButton.removeState(LV_STATE_CHECKED);
	
	usbButton.create(background.getObject());
        
	usbButton.setSize(RATIO_X*24, RATIO_Y*24);
	usbButton.setAlignment(LV_ALIGN_CENTER, RATIO_X*0, RATIO_Y*0);
	//usbButton.setStyleRotation(450);
	
	usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
        usbButton.setSimpleCallback(&this->toggleUsbMode);
	if(palcome_message_mode == 2)
		usbButton.addState(LV_STATE_CHECKED);
	else
		usbButton.removeState(LV_STATE_CHECKED);

*/

	/*triangle.tPoints.a[0] = 0;
	triangle.tPoints.a[1] = 0;
	triangle.tPoints.b[0] = SCREEN_HOR;
	triangle.tPoints.b[1] = 0;
	triangle.tPoints.c[0] = SCREEN_HOR/2;
	triangle.tPoints.c[1] = SCREEN_VIR/2;
	
	triangle.tPoints.color = triangle.colors.make(5, 25, 155);

	triangle.create(background.getObject());
	triangle.setAlignment(LV_ALIGN_TOP_LEFT, 0, 0);
	triangle.setStyleBgOpacity(0);
	triangle.setStyleBorderOpacity(0);
	triangle.fullScreen();
	triangle.setPoints();
	lv_obj_set_style_transform_rotation(triangle.getObject(), 90, 0);*/

	
	/*PalcomLabel label;
	label.create(target);
        label.setText("Select a nigger");
        label.center();*/
}

void PalcomDebugScreen::buildHomepage(lv_obj_t *target){
	PalcomObject msgContainer;
	PalcomObject msgLogContainer;
	PalcomTextarea textarea;
	PalcomButton button;
	PalcomLabel label;

	/*
	 * Message log box
	 * */
	msgLogContainer.generate(target, pal_base);
	msgLogContainer.setDefaultStyle(this->msgSenderStyle.getStyle2());
	msgLogContainer.setSize(100, 100);
	msgLogContainer.setAlignment(LV_ALIGN_TOP_LEFT, -1, -1);	
	msgLogContainer.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	msgLogContainer.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);


	/*
	 * Message sender box
	 * */
	msgContainer.generate(target, pal_base);
	msgContainer.setDefaultStyle(this->msgSenderStyle.getStyle());
	msgContainer.setSize(90, 25);
	msgContainer.setAlignment(LV_ALIGN_BOTTOM_LEFT, 10, -13);
	msgContainer.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	msgContainer.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	
	textarea.create(msgContainer.getObject());
	textarea.setDefaultStyle(this->msgSenderStyle.getStyle3());
	textarea.setOneLine(false);
	textarea.setSize(250, 50);
	textarea.setAlignment(LV_ALIGN_BOTTOM_LEFT, -10, 10);

	button.create(msgContainer.getObject());
	button.setDefaultStyle(this->msgSenderStyle.getStyle4());
	button.setSize(10, 100);
	button.setAlignment(LV_ALIGN_BOTTOM_RIGHT, 10, 0);
	button.setSimpleCallback(&this->sendMessage);

	label.create(button.getObject());
	label.center();
	label.setText(LV_SYMBOL_RIGHT);
}

void PalcomDebugScreen::reset(void){
	this->setBuildRequired(true);
	this->globalDestroy();
	this->destroy();
	this->clearScreenError();
}

void PalcomDebugScreen::generateObjects(void){
	lv_obj_t *screen = this->genScreen();
	this->setFullScreen();
	this->unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

	PalcomLabel label;
	PalcomTileview tileView;
	
	const lv_img_dsc_t *img_src[1] = {&spaceAI2};
        this->setBgImage(img_src);

	tileView.create(screen);
	tileView.setDefaultStyle(this->tileStyle.getStyle());
	tileView.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	
	lv_obj_t *tile1 = tileView.newTile(1, 1, LV_DIR_TOP | LV_DIR_LEFT | LV_DIR_RIGHT);
	this->buildHomepage(tile1);

	lv_obj_t *tile2 = tileView.newTile(1, 0, LV_DIR_BOTTOM);
	this->buildModeSelect(tile2);

	lv_obj_t *tile3 = tileView.newTile(0, 1, LV_DIR_RIGHT);
	label.create(tile3);
    	label.setText("About");
    	label.center();

	lv_obj_t *tile4 = tileView.newTile(2, 1, LV_DIR_LEFT);
	label.create(tile4);
    	label.setText("Settings");
    	label.center();

	tileView.setTile(1, 1, false);

	this->currentMode = palcome_message_mode;
}

int PalcomDebugScreen::run(void){
	this->buildCheck();
	this->execute();
	if(this->currentMode != palcome_message_mode){
		this->currentMode = palcome_message_mode;
		if(this->currentMode == 0){
			meshButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			
			nodeButton.removeState(LV_STATE_CHECKED);
			usbButton.removeState(LV_STATE_CHECKED);
		}else if(this->currentMode == 1){
			meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);

	                meshButton.removeState(LV_STATE_CHECKED);
			usbButton.removeState(LV_STATE_CHECKED);
		}else if(this->currentMode == 2){
			meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);

	                meshButton.removeState(LV_STATE_CHECKED);
			nodeButton.removeState(LV_STATE_CHECKED);
		}

		
	}
	return 0;
}
