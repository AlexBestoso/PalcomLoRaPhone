#define STORAGE_INSTR_INITALIZE 1

class Storage{
	private:
		struct task_queue_task task;
		int spaceType;

		bool pop();
	public:
	Storage();
	~Storage();

	void initalize(void);
	bool fetchTask(void);
	bool runTask(void);
};
