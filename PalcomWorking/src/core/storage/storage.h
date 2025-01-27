#define STORAGE_INSTR_INITALIZE 1

class Storage{
	private:
		struct task_queue_task task;
		int spaceType;

		bool pop();
		bool storeMessage(void);

		File _fd; // This variable should be swapped with a pointer to an initalized file management class.
	public:
		Storage();
		~Storage();

		bool fetchTask(void);
		bool runTask(void);
};
