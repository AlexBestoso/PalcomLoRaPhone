/*
 * Useage : Error code of 0x00 means that the error can be ignored.
 * */
#define CORE_ERR_TASK 0x100
#define ERR_TASK_CREATE (CORE_ERR_TASK + 0x1)
#define ERR_TASK_SEMAPHORE (CORE_ERR_TASK + 0x2)

#define CORE_ERR_TQ 0x200
#define ERR_TQ_SEMAPHORE (CORE_ERR_TQ + 0x1)
#define ERR_TQ_POP (CORE_ERR_TQ + 0x2)
#define ERR_TQ_PUSH (CORE_ERR_TQ + 0x3)
class CoreException : public std::exception{
	private:
		String errorMessage;
		uint32_t _errorCode;
	public:
		CoreException(String msg, uint8_t errorCode);
	
		String what(void);
	
		uint32_t errorCode(void);
	
		const char *codeTranslate(void);

		void out(void);
  		void halt(void);
};
