/*
 * Useage : Error code of 0x00 means that the error can be ignored.
 * */
#define CORE_ERR_TASK 0x100
#define ERR_TASK_CREATE (CORE_ERR_TASK + 0x1)
#define ERR_TASK_SEMAPHORE (CORE_ERR_TASK + 0x2)
#define ERR_TASK_NULL (CORE_ERR_TASK + 0x3)

#define CORE_ERR_TQ 0x200
#define ERR_TQ_SEMAPHORE (CORE_ERR_TQ + 0x1)
#define ERR_TQ_POP (CORE_ERR_TQ + 0x2)
#define ERR_TQ_PUSH (CORE_ERR_TQ + 0x3)
class CoreException : public std::exception{
	private:
		String errorMessage;
		uint32_t _errorCode;
		String className;
                String functionName;
                String from;
	public:
		CoreException(String msg, uint8_t errorCode);
		CoreException(const CoreException& e, String fName, String msg, uint8_t errorCode);
		CoreException(String fName, String msg, uint8_t errorCode);
                CoreException(String msg, String classN, String funcN, uint8_t errorCode);
	
		void setClassName(const char *name);

		String what(void);
		String where(void);
	
		uint32_t errorCode(void);
	
		const char *codeTranslate(void);

		void out(void);
  		void halt(void);
		void log(void);
};
