#define TASK_SPACE_COMMS 1
#define TASK_SPACE_GRAPHICS 2
#define TASK_SPACE_STORAGE 3
#define TASK_SPACE_USERINPUT 4
#define TASK_SPACE_USB 5
#define TASK_SPACE_GOD 99

struct task_queue_task{ // 32 bits in size
	bool active=false;
	uint8_t to=0;
	uint8_t from=0;
	uint8_t instruction=0;
};

#define TASK_QUEUE_MAX 1024
class TaskQueue{
	private:
		bool locked;
		struct task_queue_task tasks[TASK_QUEUE_MAX];
		size_t taskCount;
		bool lock(void);
		void unlock(void);
	public:
		TaskQueue(void);
		~TaskQueue();
	
		bool mine(int spaceID);
		void push(struct task_queue_task task);
		void push(uint8_t to, uint8_t from, uint8_t instruction);
		struct task_queue_task pop(int spaceId);
		void exec(void);
		void clear(void);
		int getTaskCount(void);
		struct task_queue_task buildTask(uint8_t to, uint8_t from, uint8_t instruction);
};
