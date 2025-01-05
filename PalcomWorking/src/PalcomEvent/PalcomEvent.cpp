class PalcomEvent{
	private:
		lv_event_t *targetEvent = NULL;

		lv_event_t *validateTarget(lv_event_t *src){
			if(src == NULL){
                                if(this->targetEvent == NULL)
                                        throw CoreException("PalcomEvent::validateTarget() - (lv_event_t *) src is null.", 0x01);
                                else
                                        src = targetEvent;
                        }

			return src;
		}
	public:
		PalcomEvent(void){
			targetEvent = NULL;
		}
		PalcomEvent(lv_event_t *e){
			this->targetEvent = e;
		}

		lv_result_t send(lv_event_list_t *list, bool preprocess){
			if(targetEvent == NULL)
				throw CoreException("PalcomEvent::send() - target event is null.", 0x01);
			return lv_event_send(list, this->targetEvent, preprocess);
		}

		lv_event_dsc_t *add(lv_event_list_t *list, lv_event_cb_t cb, lv_event_code_t filter, void *userData){
			if(list == NULL)
				throw CoreException("PalcomEvent::add() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_add(list, cb, filter, userData);
		}

		bool removeDsc(lv_event_list_t *list, lv_event_dsc_t *dsc){
			if(list == NULL)
				throw CoreException("PalcomEvent::remove() - (lv_event_list_t *) list is null.", 0x01);
			if(dsc == NULL)
				throw CoreException("PalcomEvent::remove() - (lv_event_dsc_t *) dsc is null.", 0x02);
			return lv_event_remove_dsc(list, dsc);
		}

		uint32_t getCount(lv_event_list_t *list){
			if(list == NULL)
				throw CoreException("PalcomEvent::getCount() - (lv_event_list_t *) list is null.", 0x03);
			return lv_event_get_count(list);
		}

		lv_event_dsc_t *getDsc(lv_event_list_t *list, uint32_t index){
			if(list == NULL)
				throw CoreException("PalcomEvent::getDsc() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_get_dsc(list, index);
		}

		lv_event_cb_t getDscCallback(lv_event_dsc_t *dsc){
			if(dsc == NULL)
				throw CoreException("PalcomEvent::getDscCallback() - (lv_event_dsc_t *) dsc is null.", 0x01);
			return lv_event_dsc_get_cb(dsc);
		}

		void *getDscUserData(lv_event_dsc_t *dsc){
			if(dsc == NULL)
				throw CoreException("PalcomEvent::getDscUserData() - (lv_event_dsc_T *) dsc is null.", 0x01);
			return lv_event_dsc_get_user_data(dsc);
		}

		bool remove(lv_event_list_t *list, uint32_t index){
			if(list == NULL)
                                throw CoreException("PalcomEvent::remove() - (lv_event_list_t *) list is null.", 0x01);
			return lv_event_remove(list, index);
		}

		void removeAll(lv_event_list_t *list){
			if(list == NULL)
                                throw CoreException("PalcomEvent::getDsc() - (lv_event_list_t *) list is null.", 0x01);
			lv_event_remove_all(list);
		}

		void *getTarget(lv_event_t *src=NULL){
			src = validateTarget(src);

			return lv_event_get_target(src);
		}

		void *getCurrentTarget(lv_event_t *src=NULL){
			src = validateTarget(src);

			return lv_event_get_current_target(src);
		}

		lv_event_code_t getCode(lv_event_t *src=NULL){
			src = validateTarget(src);
			
			return lv_event_get_code(src);
		}

		void *getParam(lv_event_t *src=NULL){
			src = validateTarget(src);

			return lv_event_get_param(src);
		}

		void *getUserData(lv_event_t *src=NULL){
			src = validateTarget(src);

			return lv_event_get_user_data(src);
		}

		void stopBubbling(lv_event_t *src=NULL){
			src = validateTarget(src);

			lv_event_stop_bubbling(src);
		}

		void stopProcessing(lv_event_t *src=NULL){
			src = validateTarget(src);

			lv_event_stop_processing(src);
		}

		uint32_t registerId(void){
			return lv_event_register_id();
		}

};
