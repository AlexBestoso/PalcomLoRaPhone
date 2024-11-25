#define TASK_SPACE_COMMS 1
#define TASK_SPACE_GRAPHICS 2
#define TASK_SPACE_STORAGE 3
#define TASK_SPACE_USERINPUT 4
#define TASK_SPACE_USB 5
#define TASK_SPACE_GOD 99

struct task_queue_task{ // 32 bits in size
	uint16_t id;
	uint8_t to;
	uint8_t from;
	uint8_t instruction;
};

#define TASK_QUEUE_MAX 1024
SemaphoreHandle_t queueSemaphore = NULL;
class TaskQueue{
	private:
		struct task_queue_task tasks[TASK_QUEUE_MAX];
		bool lock(void);
		bool unlock(void);
	public:
		TaskQueue(void);
		~TaskQueue();
	
		void push(struct task_queue_task task);
		struct task_queue_task pop(int id);
		void exec(void);
		void clear(void);
		int getTaskCount(void);
		struct task_queue_task buildTask(uint8_t to, uint8_t from, uint8_t instruction);
};
