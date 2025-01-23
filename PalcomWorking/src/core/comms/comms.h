#define BOARD_SDCARD_CS     39
#define BOARD_TFT_CS        12
#define RADIO_CS_PIN        9
class Comms{
        private:
                struct task_queue_task task;
                int spaceType;
		int activePage;

                bool pop();
		bool sendMsg(void);
		bool recvMsg(void);	
	
		LoRaSnake *lsnake;

        public:
        	Comms(void);
		Comms(LoRaSnake *l);
        	~Comms();

        	bool fetchTask(void);
        	bool runTask(void);

};
