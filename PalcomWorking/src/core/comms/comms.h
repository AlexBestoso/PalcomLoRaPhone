class Comms{
        private:
                struct task_queue_task task;
                int spaceType;
		int activePage;

                bool pop();
		bool sendMsg(void);
		bool recvMsg(void);	
		bool sendMsgNode(void);
	
		LoRaSnake *lsnake;

        public:
        	Comms(void);
		Comms(LoRaSnake *l);
        	~Comms();

        	bool fetchTask(void);
        	bool runTask(void);

};
