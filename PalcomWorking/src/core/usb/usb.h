static void usbEventCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

class Usb{
	private:
		struct task_queue_task task;
		int spaceType;

		bool pop();
	public:
		Usb();
		~Usb();

		void init(void);
		bool fetchTask(void);
		bool runTask(void);
};
