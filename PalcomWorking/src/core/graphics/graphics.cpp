#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <src/taskQueue/taskQueue.h>

#include <src/PalcomStyle/PalcomStyle.h>
#include <src/PalcomStyle/styles/styles.h>

#include <src/PalcomObject/PalcomObject.h>
#include <src/PalcomObject/Label/Label.h>
#include <src/PalcomObject/Button/Button.h>
#include <src/PalcomObject/Triangle/Triangle.h>

#include <src/PalcomScreen/PalcomScreen.h>
#include <src/PalcomScreen/setMsgMode/setMsgMode.h>
#include <src/PalcomScreen/DebugScreen/DebugScreen.h>
#include "./graphics.h"

extern TaskQueue taskQueue;
//Private
bool Graphics::pop(void){
        struct task_queue_task t = taskQueue.pop(this->spaceType);
        if(!t.active)
                return false;
        this->task = t;
        return true;
}

//Public
Graphics::Graphics(void){
        this->spaceType = TASK_SPACE_GRAPHICS;
	this->activePage = 1;
}
Graphics::~Graphics(){}


bool Graphics::fetchTask(void){
        return this->pop();
}

bool Graphics::runTask(void){
        switch(this->task.instruction){
                case GRAPHICS_INSTR_SETUP:
			this->activePage = GRAPHICS_INSTR_SETUP;
                        return true;
                break;
		case GRAPHICS_INSTR_HOMEPAGE:
			this->activePage = GRAPHICS_INSTR_HOMEPAGE;
			return true;
		break;
		case GRAPHICS_INSTR_PUSH_MSGM:{
                        Serial.printf("Pushing my sent message to the display...\n");
                }
                return true;
        }
        return false;
}

bool Graphics::exec(bool destroy){
	switch(this->activePage){
		case GRAPHICS_INSTR_SETUP:{
			if(destroy)
				this->setMsgModeScreen.reset();
			this->setMsgModeScreen.run();
		}
		return true;
		
		case GRAPHICS_INSTR_HOMEPAGE:{
			if(destroy)
				pds.reset();
			pds.run();
		}
		return true;
		default:
			Serial.printf("[%d] Must Show Error Page.\n", this->activePage);
			return false;	
	}
	return true;
}
