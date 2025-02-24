class Storage{
	private:
		struct task_queue_task task;
		int spaceType;

		bool pop();
		bool storeMessage(bool mine);
		void relayMsgToDisplay(bool mine);

		File _fd; // This variable should be swapped with a pointer to an initalized file management class.
	public:
		Storage();
		~Storage();

		bool fetchTask(void);
		bool runTask(void);
};
