class PalcomEvent{
	private:
		lv_event_t *targetEvent;

		lv_event_t *validateTarget(lv_event_t *src);
	public:
		PalcomEvent(void);
		PalcomEvent(lv_event_t *e);

		lv_result_t send(lv_event_list_t *list, bool preprocess);
		lv_event_dsc_t *add(lv_event_list_t *list, lv_event_cb_t cb, lv_event_code_t filter, void *userData);
		bool removeDsc(lv_event_list_t *list, lv_event_dsc_t *dsc);
		uint32_t getCount(lv_event_list_t *list);
		lv_event_dsc_t *getDsc(lv_event_list_t *list, uint32_t index);
		lv_event_cb_t getDscCallback(lv_event_dsc_t *dsc);
		void *getDscUserData(lv_event_dsc_t *dsc);
		bool remove(lv_event_list_t *list, uint32_t index);
		void removeAll(lv_event_list_t *list);
		void *getTarget(lv_event_t *src);
		void *getCurrentTarget(lv_event_t *src);
		lv_event_code_t getCode(lv_event_t *src);
		void *getParam(lv_event_t *src);
		void *getUserData(lv_event_t *src);
		void stopBubbling(lv_event_t *src);
		void stopProcessing(lv_event_t *src);

		void *getTarget(void);
		void *getCurrentTarget(void);
		lv_event_code_t getCode(void);
		void *getParam(void);
		void *getUserData(void);
		void stopBubbling(void);
		void stopProcessing(void);


		uint32_t registerId(void);
};
