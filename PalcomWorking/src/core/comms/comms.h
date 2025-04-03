class Comms{
        private:
                struct task_queue_task task;
                int spaceType;
		int activePage;

                bool pop();
		bool sendMsg(void);
		bool recvMsg(void);	
		bool sendMsgNode(void);
		bool recvMsgNode(void);
	
		LoRaSnake *lsnake;

		unsigned char iv[16];
                unsigned char inKey[32];
                unsigned char outKey[32];
                unsigned char padding[32];
                unsigned char*core_routing_key;
                size_t core_routing_key_size;
                Cryptography *cryptography;

                void deriveRoutingIv(void);
                void deriveRoutingRecvKey(void);
                void deriveRoutingSendKey(void);
                void deriveRoutingPadding(void);

		bool validatePadding(unsigned char *data, size_t dataSize);

        public:
        	Comms(void);
		Comms(LoRaSnake *l);
        	~Comms();

		void init(Cryptography *c, unsigned char*core_routing_key, size_t core_routing_key_size);

        	bool fetchTask(void);
        	bool runTask(void);

};
