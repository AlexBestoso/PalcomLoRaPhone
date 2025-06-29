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
#include <src/PalcomObject/Image/Image.h>
#include <src/PalcomObject/Line/Line.h>
#include <src/PalcomObject/Triangle/Triangle.h>
#include <src/PalcomObject/Dropdown/Dropdown.h>
#include <src/PalcomObject/tabview/tabview.h>

#include <src/PalcomScreen/PalcomScreen.h>
#include <src/taskQueue/taskQueue.h>

#include <src/PalcomSettings/settings.h>

#define USER_BUF_SIZE 256

extern const lv_image_dsc_t spaceAI2;
extern int PalcomScreenError;
extern int palcome_message_mode;
extern TaskQueue taskQueue;
extern uint8_t userBuffer[USER_BUF_SIZE];
extern size_t userBufferSize;
extern lv_obj_t *keyboardFocusedObj;
extern char displayed_messages[10][257];
extern int displayed_page;
extern palcom_partition_t COMMS_SETTINGS;

static lv_obj_t *settings_objs[6];

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
		t.instruction = palcome_message_mode == 0 ? COMMS_INSTR_SEND_USB : palcome_message_mode == 1 ? COMMS_INSTR_SEND_LORA: COMMS_INSTR_SEND_WIFI;
		const char *msg = textarea.getText();
		int onset = strlen((const char *)COMMS_SETTINGS.pre), endset=256-strlen((const char *)COMMS_SETTINGS.app);
	
		for(int i=onset; i<endset && i<textarea.getTextSize(); i++){
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

void PalcomDebugScreen::applySettings(lv_event_t *e){
	char buf[32];
	PalcomEvent event(e);
	PalcomDropdown dropdown;
	PalcomTextarea textarea;

	if(event.getCode() != LV_EVENT_PRESSED)
		return;
	lv_obj_t *grabber = (lv_obj_t *)event.getUserData();
	if(grabber == NULL){
		Serial.printf("Data is null, not applying settings.\n");
		return;
	}

	PalcomSettings settings;	

	dropdown.setObject(settings_objs[0]);
	dropdown.getSelection(buf, 32);
	const char *encryption = buf;
	settings.setEncryption(encryption);


	dropdown.setObject(settings_objs[1]);
	dropdown.getSelection(buf, 32);
	const char *encoding = buf;
        settings.setEncoding(encoding);

	dropdown.setObject(settings_objs[2]);
	dropdown.getSelection(buf, 32);
	const char *sendRoute = buf;
        settings.setSendRoute(sendRoute);

	dropdown.setObject(settings_objs[3]);
	dropdown.getSelection(buf, 32);
	const char *recvRoute = buf;
        settings.setRecvRoute(recvRoute);
	
	textarea.setObject(settings_objs[4]);
	const char *prepend = textarea.getText();
        settings.setPrepend(prepend);
		
	textarea.setObject(settings_objs[5]);
	const char *append = textarea.getText();
        settings.setAppend(append);
	
	settings.update();
	COMMS_SETTINGS = settings.getPartition(true);
}

PalcomDebugScreen::PalcomDebugScreen(void){
	this->buttonStyle.initStyle();
	PalcomScreenError = 0;
}

PalcomDebugScreen::~PalcomDebugScreen(){

}


void PalcomDebugScreen::buildLoRaSettings(lv_obj_t *mainSettings){
	//void setBandwidth(float v);
        //void setSpreadFactor(int v);                       
        //void setCodingRate(int v);
        //void setSyncWord(int s, int ctrl);                 
        //void setPreambleLength(size_t v);
	PalcomObject base;
	PalcomTextarea textarea;
	PalcomLabel label;
	PalcomDropdown dropdown;
	PalcomButton button;

	int fieldOffset = 27*2+2;
	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*0));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Bandwidth: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("7.8\n10.4\n15.6\n20.8\n31.25\n41.7\n62.5\n125.0\n250.0\n500.0");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);


	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*1));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Spread Factor: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("5\n6\n7\n8\n9\n10\n11\n12");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);


	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*2));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Coding Rate: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("5\n6\n7\n8");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);


	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*3));
	base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Sync Word: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	textarea.create(base.getObject());
	textarea.setMaxLength(16);
	textarea.setOneLine(true);
	textarea.setSize(150, 36);
	textarea.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);


	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*4));
	base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Control Bit: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	textarea.create(base.getObject());
	textarea.setMaxLength(16);
	textarea.setOneLine(true);
	textarea.setSize(150, 36);
	textarea.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);

	
	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*5));
	base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	label.create(base.getObject());
	label.setText("Preamble Len: ");
	label.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	textarea.create(base.getObject());
	textarea.setMaxLength(16);
	textarea.setOneLine(true);
	textarea.setSize(150, 36);
	textarea.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);

}
void PalcomDebugScreen::buildMainSettings(lv_obj_t *mainSettings){
	PalcomSettings settings;
	palcom_partition_t settings_data;
	PalcomLabel title;
	PalcomDropdown dropdown;
	PalcomObject base;
	PalcomTextarea textarea;
	PalcomButton button;

	settings_data = settings.getPartition(true);

	title.create(mainSettings);
	title.setText("USB Settings");
	title.setAlignment(LV_ALIGN_TOP_MID, 0, 0);

	int fieldOffset = 27*2+2;
	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*0));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Encryption: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("Disabled\nAES-XTS\nAES-OFB\nAES-CTR\nAES-ECB\nAES-CBC");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	dropdown.setSelection(settings_data.encryption);
	settings_objs[0] = dropdown.getObject();
	
	
	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*1));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Encoding: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("Disabled\nBase64");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	dropdown.setSelection(settings_data.encoding);
	settings_objs[1] = dropdown.getObject();

	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*2));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Send Route: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("Usb\nLoRa\nWiFi");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	dropdown.setSelection(settings_data.send_route);
	settings_objs[2] = dropdown.getObject();

	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*3));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Recv Route: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	dropdown.create(base.getObject());
	dropdown.setList("Usb\nLoRa\nWiFi");
	dropdown.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	dropdown.setSelection(settings_data.recv_route);
	settings_objs[3] = dropdown.getObject();

	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*4));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Prepend: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	textarea.create(base.getObject());
	textarea.setMaxLength(16);
	textarea.setOneLine(true);
	textarea.setSize(150, 36);
	textarea.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	textarea.setText((const char *)settings_data.pre);
	settings_objs[4] = textarea.getObject();

	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*5));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	title.create(base.getObject());
	title.setText("Append: ");
	title.setAlignment(LV_ALIGN_TOP_LEFT, 0, 10-3);
	textarea.create(base.getObject());
	textarea.setMaxLength(16);
	textarea.setOneLine(true);
	textarea.setSize(150, 36);
	textarea.setAlignment(LV_ALIGN_TOP_RIGHT, 0, -3);
	textarea.setText((const char *)settings_data.app);
	settings_objs[5] = textarea.getObject();
	
	base.generate(mainSettings, pal_base);
	base.setSize(100, 27);
	base.setAlignment(LV_ALIGN_TOP_LEFT, -5, 0+(fieldOffset*6));
        base.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        base.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	button.create(base.getObject());
        title.create(button.getObject());
        title.setText("Apply");
        title.center();
        button.setLabel(title);
        button.setDefaultStyle(this->buttonStyle.getStyle());
        button.setPressedStyle(this->buttonStyle.getPressedStyle());
        button.setSize(45, 72+27*2);
        button.setAlignment(LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        button.setParamCallback(&this->applySettings, (void *)settings_objs);

}
void PalcomDebugScreen::buildUsbSettings(lv_obj_t *Target){
	PalcomTabview tabview;
	
	lv_obj_remove_flag(Target, LV_OBJ_FLAG_SCROLLABLE);
	tabview.create(Target);
	tabview.center();
	tabview.fullScreen();
	tabview.tabBarPositionTop();
	tabview.setTabBarSize(35);
	tabview.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
	tabview.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	
	lv_obj_t *mainSettings = tabview.newTab("Main");
	lv_obj_t *loraSettings = tabview.newTab("LoRa");

	this->buildMainSettings(mainSettings);
	this->buildLoRaSettings(loraSettings);

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
}

void PalcomDebugScreen::buildHomepage(lv_obj_t *target){
	PalcomObject msgContainer;
	PalcomObject msgLogContainer;
	PalcomObject message;
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
	msgLogContainer.setFlag(LV_OBJ_FLAG_SCROLLABLE);

	/*
	 * Draw Messages
	 * */
	int lastSpot = 0;
	for(int i=0; i<10; i++){
		if(displayed_messages[i][0] >= 2){
			continue;
		}
		lastSpot = i;
		message.generate(msgLogContainer.getObject(), pal_base);
		message.setSize(80, 30);
		message.setAlignment((displayed_messages[i][0] == 1) ? LV_ALIGN_TOP_RIGHT : LV_ALIGN_TOP_LEFT, 0, (60*i)+(10*i));
		message.setScrollMode(LV_SCROLLBAR_MODE_OFF);
		//message.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);
		
		label.create(message.getObject());
		label.setLongMode(LV_LABEL_LONG_WRAP);
		label.setWidth(75*3);
		String msg = "";
		for(int j=0; j<256; j++){
			if(j < 2 && displayed_messages[i][j] < 2)
				continue;
			msg += displayed_messages[i][j];
			
		}
		label.setText(msg.c_str());
		
	}
	message.generate(msgLogContainer.getObject(), pal_base);
        message.setSize(90, 30);
        message.setAlignment(LV_ALIGN_TOP_RIGHT, 0, (60*(lastSpot+1))+(10*(lastSpot+1)));
	message.setStyleBgOpacity(-1);
	lv_obj_scroll_to_view(message.getObject(), (lv_anim_enable_t)false);

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

void PalcomDebugScreen::buildAboutpage(lv_obj_t *target){
	PalcomObject background;
	PalcomLabel label;

	background.generate(target, pal_base);
	background.setDefaultStyle(this->msgSenderStyle.getStyle2());
	background.setSize(100, 100);
	background.setAlignment(LV_ALIGN_TOP_LEFT, -1, -1);
	background.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	background.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

	String text = "BT Palcom Messenger\nVersion: ";
	text += SYS_VERSION;
	label.create(background.getObject());
    	label.setText(text.c_str());
    	label.center();

	
}
	
void PalcomDebugScreen::buildSettingspage(lv_obj_t *target){
	 PalcomLabel label;
	PalcomObject background;

        background.generate(target, pal_base);
        background.setDefaultStyle(this->msgSenderStyle.getStyle2());
        background.setSize(100, 100);
        background.setAlignment(LV_ALIGN_TOP_LEFT, -1, -1);
        
	switch(palcome_message_mode){
		case 1:{
			background.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        		background.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

			label.create(background.getObject());
		    	label.setText("Node Settings");
		    	label.center();
		}
		break;
		case 2:{
			this->buildUsbSettings(background.getObject());
		}
		break;
		default:{
			background.setScrollMode(LV_SCROLLBAR_MODE_OFF);
        		background.unsetFlag(LV_OBJ_FLAG_SCROLLABLE);

			label.create(background.getObject());
		    	label.setText("LoRa Settings");
		    	label.center();
		}
		break;
	}
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
	
	const lv_img_dsc_t *img_src[1] = {&spaceAI2};
        this->setBgImage(img_src);

	tileView.create(screen);
	tileView.setDefaultStyle(this->tileStyle.getStyle());
	tileView.setScrollMode(LV_SCROLLBAR_MODE_OFF);
	
	tile1 = tileView.newTile(1, 0, LV_DIR_BOTTOM | LV_DIR_LEFT | LV_DIR_RIGHT);
	this->buildHomepage(tile1);

	tile2 = tileView.newTile(1, 1, LV_DIR_TOP);
	this->buildModeSelect(tile2);

	tile3 = tileView.newTile(0, 0, LV_DIR_RIGHT);
	this->buildAboutpage(tile3);
	
	tile4 = tileView.newTile(2, 0, LV_DIR_LEFT);
	this->buildSettingspage(tile4);
	
	tileView.setTile(1, 0, false);

	this->currentMode = palcome_message_mode;
}

int PalcomDebugScreen::run(void){
	this->buildCheck();
	this->execute();
	if(this->currentMode != palcome_message_mode){
		this->currentMode = palcome_message_mode;
		if(this->currentMode == 0){ // Mesh mode.
			meshButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			
			nodeButton.removeState(LV_STATE_CHECKED);
			usbButton.removeState(LV_STATE_CHECKED);
			this->reset();
			this->buildCheck();
			tileView.setTile(1, 1, false);
			this->execute();
		}else if(this->currentMode == 1){
			meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.setFlag(LV_OBJ_FLAG_CHECKABLE);

	                meshButton.removeState(LV_STATE_CHECKED);
			usbButton.removeState(LV_STATE_CHECKED);
			this->reset();
			this->buildCheck();
			tileView.setTile(1, 1, false);
			this->execute();
		}else if(this->currentMode == 2){
			meshButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			nodeButton.setFlag(LV_OBJ_FLAG_CHECKABLE);
			usbButton.unsetFlag(LV_OBJ_FLAG_CHECKABLE);

	                meshButton.removeState(LV_STATE_CHECKED);
			nodeButton.removeState(LV_STATE_CHECKED);
			this->reset();
			this->buildCheck();
			tileView.setTile(1, 1, false);
			this->execute();
		}

		
	}else{
		// Handl message display logic...
	}
	return 0;
}
