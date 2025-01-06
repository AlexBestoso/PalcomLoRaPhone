#include <Arduino.h>
#include <lvgl.h>
#include <cstdint>

#include "../../PalcomStyle/PalcomStyle.h"
#include "../../PalcomStyle/styles/styles.h"
#include "../../PalcomObject/PalcomObject.h"
#include "../../PalcomObject/Label/Label.h"
#include "../../PalcomObject/Button/Button.h"
#include <src/PalcomObject/Textarea/Textarea.h>
#include "../../PalcomObject/Tileview/Tileview.h"
#include "../../PalcomObject/Image/Image.h"

#include "../../PalcomScreen/PalcomScreen.h"
#include "../../taskQueue/taskQueue.h"

#define USER_BUF_SIZE 256

extern const lv_image_dsc_t spaceAI2;
extern int PalcomScreenError;
extern int palcome_message_mode;
extern TaskQueue taskQueue;
extern uint8_t userBuffer[USER_BUF_SIZE];
extern size_t userBufferSize;
extern lv_obj_t *keyboardFocusedObj;

#include "./DebugScreen.h"

PalcomDebugScreen::PalcomDebugScreen(void){
	this->buttonStyle.initStyle();
	PalcomScreenError = 0;
}

PalcomDebugScreen::~PalcomDebugScreen(){

}

void PalcomDebugScreen::buildHomepage(lv_obj_t *target){
	PalcomObject msgContainer;
	PalcomObject msgLogContainer;
	PalcomTextarea textarea;

	msgLogContainer.generate(target, pal_base);
	msgLogContainer.setDefaultStyle(this->msgSenderStyle.getStyle2());
	msgLogContainer.setSize(101, 100);
	msgLogContainer.setAlignment(LV_ALIGN_TOP_LEFT, -1, -1);	

	msgContainer.generate(target, pal_base);
	msgContainer.setDefaultStyle(this->msgSenderStyle.getStyle());
	msgContainer.setSize(90, 25);
	msgContainer.setAlignment(LV_ALIGN_BOTTOM_LEFT, 10, -13);
	msgContainer.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	msgContainer.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	
	textarea.create(msgContainer.getObject());
	textarea.setDefaultStyle(this->msgSenderStyle.getStyle3());
	textarea.setOneLine(false);
	textarea.setSize(280, 50);
	textarea.setAlignment(LV_ALIGN_BOTTOM_LEFT, -10, 10);
	
	//PalcomLabel label;
	//label.create(target);
        //label.setText("Home page");
        //label.center();
	// Create background image
	// 
}

void PalcomDebugScreen::reset(void){
	this->setBuildRequired(true);
	this->globalDestroy();
	this->destroy();
	this->clearScreenError();
}

/*
 TODO: 	1) Create classes for controlling each of the specific screens.
 	2) Starting with message viewm, create the following:
		- logic to control which medium send messages use
		- Logic for controlling which folder to store respective received messages
		- generate default encryption keys for each mode of messaging
		- diffientiate between received messages and sent messages.
		- Message pagination for when there's large amounts of data.
*/
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
	
	lv_obj_t *tile1 = tileView.newTile(1, 1, LV_DIR_TOP | LV_DIR_LEFT | LV_DIR_RIGHT);
	this->buildHomepage(tile1);

	lv_obj_t *tile2 = tileView.newTile(1, 0, LV_DIR_BOTTOM);
	label.create(tile2);
    	label.setText("Mode Select");
    	label.center();

	lv_obj_t *tile3 = tileView.newTile(0, 1, LV_DIR_RIGHT);
	label.create(tile3);
    	label.setText("About");
    	label.center();

	lv_obj_t *tile4 = tileView.newTile(2, 1, LV_DIR_LEFT);
	label.create(tile4);
    	label.setText("Settings");
    	label.center();

	tileView.setTile(1, 1, false);
}

int PalcomDebugScreen::run(void){
	this->buildCheck();
	this->execute();
	return 0;
}
