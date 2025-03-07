class SdSnake{
	private:

		File _fd;
	public:
		SdSnake(void);
		~SdSnake();
		void rm(const char *target);
		bool writeFile(const char *fname, char *data, size_t dataSize);
		bool readFile(const char *fname, char *outData, size_t outDataSize);
		bool init(void);

		
};
