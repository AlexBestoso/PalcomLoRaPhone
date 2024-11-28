#define GRAPHICS_INSTR_SETUP 1

class Graphics{
        private:
                struct task_queue_task task;
                int spaceType;
		int activePage;

                bool pop();

		PalcomDebugScreen pds;

        public:
        	Graphics();
        	~Graphics();

        	bool fetchTask(void);
        	bool runTask(void);

		bool exec(bool destroy);
};
