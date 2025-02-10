#include <Arduino.h>
#include <cstdint>
#include <lvgl.h>

#include <src/error/error.h>

#include "./PalcomEvent.h"
		lv_event_t *PalcomEvent::validateTarget(lv_event_t *src){
			if(src == NULL){
                                if(this->targetEvent == NULL)
                                        throw CoreException("PalcomEvent::validateTarget() - (lv_event_t *) src is null.", 0x01);
                                else
                                        src = targetEvent;
                        }

			return src;
		}

		PalcomEvent::PalcomEvent(void){
			targetEvent = NULL;
		}
		PalcomEvent::PalcomEvent(lv_event_t *e){
			this->targetEvent = e;
		}

		lv_result_t PalcomEvent::send(lv_event_list_t *list, bool preprocess){
			if(targetEvent == NULL)
				throw CoreException("PalcomEvent::send() - target event is null.", 0x01);
			return lv_event_send(list, this->targetEvent, preprocess);
		}

		lv_event_dsc_t *PalcomEvent::add(lv_event_list_t *list, lv_event_cb_t cb, lv_event_code_t filter, void *userData){
			if(list == NULL)
				throw CoreException("PalcomEvent::add() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_add(list, cb, filter, userData);
		}

		bool PalcomEvent::removeDsc(lv_event_list_t *list, lv_event_dsc_t *dsc){
			if(list == NULL)
				throw CoreException("PalcomEvent::remove() - (lv_event_list_t *) list is null.", 0x01);
			if(dsc == NULL)
				throw CoreException("PalcomEvent::remove() - (lv_event_dsc_t *) dsc is null.", 0x02);
			return lv_event_remove_dsc(list, dsc);
		}

		uint32_t PalcomEvent::getCount(lv_event_list_t *list){
			if(list == NULL)
				throw CoreException("PalcomEvent::getCount() - (lv_event_list_t *) list is null.", 0x03);
			return lv_event_get_count(list);
		}

		lv_event_dsc_t *PalcomEvent::getDsc(lv_event_list_t *list, uint32_t index){
			if(list == NULL)
				throw CoreException("PalcomEvent::getDsc() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_get_dsc(list, index);
		}

		lv_event_cb_t PalcomEvent::getDscCallback(lv_event_dsc_t *dsc){
			if(dsc == NULL)
				throw CoreException("PalcomEvent::getDscCallback() - (lv_event_dsc_t *) dsc is null.", 0x01);
			return lv_event_dsc_get_cb(dsc);
		}

		void *PalcomEvent::getDscUserData(lv_event_dsc_t *dsc){
			if(dsc == NULL)
				throw CoreException("PalcomEvent::getDscUserData() - (lv_event_dsc_T *) dsc is null.", 0x01);
			return lv_event_dsc_get_user_data(dsc);
		}

		bool PalcomEvent::remove(lv_event_list_t *list, uint32_t index){
			if(list == NULL)
                                throw CoreException("PalcomEvent::remove() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_remove(list, index);
		}

		void PalcomEvent::removeAll(lv_event_list_t *list){
			if(list == NULL)
                                throw CoreException("PalcomEvent::getDsc() - (lv_event_list_t *) list is null.", 0x01);
			lv_event_remove_all(list);
		}

		void *PalcomEvent::getTarget(void){
			return this->getTarget(NULL);
		}

		void *PalcomEvent::getTarget(lv_event_t *src){
			src = validateTarget(src);

			return lv_event_get_target(src);
		}

		void *PalcomEvent::getCurrentTarget(lv_event_t *src){
			src = validateTarget(src);

			return lv_event_get_current_target(src);
		}
		
		void *PalcomEvent::getCurrentTarget(void){
			return this->getCurrentTarget(NULL);
                }

		lv_event_code_t PalcomEvent::getCode(lv_event_t *src){
			src = validateTarget(src);
			
			return lv_event_get_code(src);
		}

		lv_event_code_t PalcomEvent::getCode(void){
			return this->getCode(NULL);
                }

		void *PalcomEvent::getParam(lv_event_t *src){
			src = validateTarget(src);

			return lv_event_get_param(src);
		}

		void *PalcomEvent::getParam(void){
			return this->getParam(NULL);
                }

		void *PalcomEvent::getUserData(lv_event_t *src){
			src = validateTarget(src);

			return lv_event_get_user_data(src);
		}

		void *PalcomEvent::getUserData(void){
			return this->getUserData(targetEvent);
                }

		void PalcomEvent::stopBubbling(lv_event_t *src){
			src = validateTarget(src);

			lv_event_stop_bubbling(src);
		}

		void PalcomEvent::stopBubbling(void){
			this->stopBubbling(NULL);
                }

		void PalcomEvent::stopProcessing(lv_event_t *src){
			src = validateTarget(src);

			lv_event_stop_processing(src);
		}

		void PalcomEvent::stopProcessing(void){
			this->stopProcessing(NULL);
                }

		uint32_t PalcomEvent::registerId(void){
			return lv_event_register_id();
		}

