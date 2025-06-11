class GodCore{
	private:
		int setupCount;
		bool dmaEnabled;
		struct task_queue_task task;
		int spaceType;
		lv_timer_t *timer = NULL;
                lv_display_t *display = NULL;
                lv_indev_t *indev_touchpad = NULL;
                lv_indev_t *indev_keypad = NULL;
                lv_indev_t *indev_mouse = NULL;
                lv_obj_t *cursorImg = NULL;

		void run_initLora(void);
		void run_denitLoRa(void);
		
		void run_initUsb(void);
		void run_denitUsb(void);

		void run_initWifi(void);
		void run_denitWifi(void);

		void run_initPins(void);
		void run_initLcd(void);
		void run_initTouch(void);
		void run_initLvgl(void);
		void run_initSd(void);


		bool pop(void);
	public:
		GodCore(void);
		~GodCore();

		void process(void);
		bool ready(void);
		
		void initLora(void);
		void denitLoRa(void);
		
		void initUsb(void);
		void denitUsb(void);

		void initWifi(void);
		void denitWifi(void);
			
		void initPins(void);
		void initLcd(void);
		void initTouch(void);
		void initLvgl(void);
		void initSd(void);

};
