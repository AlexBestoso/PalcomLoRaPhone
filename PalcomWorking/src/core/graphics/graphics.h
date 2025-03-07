
class Graphics{
        private:
                struct task_queue_task task;
                int spaceType;
		int activePage;

                bool pop();
	
		PalcomDebugScreen pds;
		SetMsgModeScreen setMsgModeScreen;

        public:
        	Graphics();
        	~Graphics();

        	bool fetchTask(void);
        	bool runTask(void);

		bool exec(bool destroy);
};
