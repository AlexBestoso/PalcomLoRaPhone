#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>
#include <defines.h>
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
extern char displayed_messages[MSG_DISPLAY_MAX][MSG_BUFFER_MAX];
extern int displayed_page;
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
			for(int i=0; i<MSG_DISPLAY_MAX; i++){
				if(displayed_messages[i][0] >= 2){
					for(int j=1; j<MSG_BUFFER_MAX; j++) displayed_messages[i][j] = this->task.msg[j-1];
					displayed_messages[i][0] = 1;
					return true;
				}
			}

			// All spaces were filled, remove and shift down.
			for(int i=0; i<MSG_DISPLAY_MAX-1; i++){  
				for(int j=0; j<MSG_BUFFER_MAX; j++) displayed_messages[i][j] = displayed_messages[i+1][j];
			}
			
			for(int i=1; i<MSG_BUFFER_MAX; i++) displayed_messages[9][i] = this->task.msg[i-1];
			displayed_messages[9][0] = 1;
	
                	return true;
                }
		break;
		case GRAPHICS_INSTR_PUSH_MSGO:{
			for(int i=0; i<MSG_DISPLAY_MAX; i++){
                                if(displayed_messages[i][0] >= 2){
                                        for(int j=1; j<MSG_BUFFER_MAX; j++) displayed_messages[i][j] = this->task.msg[j-1];
                                        displayed_messages[i][0] = 0;
                                        return true;
                                }
                        }

                        // All spaces were filled, remove and shift down.
                        for(int i=0; i<MSG_DISPLAY_MAX-1; i++){
                                for(int j=0; j<MSG_BUFFER_MAX; j++) displayed_messages[i][j] = displayed_messages[i+1][j];
                        }

                        for(int i=1; i<MSG_BUFFER_MAX; i++) displayed_messages[9][i] = this->task.msg[i-1];
                        displayed_messages[9][0] = 0;

                        return true;
		}
		break;

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
			if(destroy){
				pds.reset();
			}
			pds.run();
		}
		return true;
		default:
			Serial.printf("[%d] Must Show Error Page.\n", this->activePage);
			return false;	
	}
	return true;
}
