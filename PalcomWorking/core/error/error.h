/*
 * Useage : Error code of 0x00 means that the error can be ignored.
 * */
#define CORE_ERR_TASK 0x100
#define ERR_TASK_CREATE (CORE_ERR_TASK + 0x1)
#define ERR_TASK_SEMAPHORE (CORE_ERR_TASK + 0x2)

class CoreException : public std::exception{
	private:
		String errorMessage = "";
		uint8_t _errorCode = 0x00;
	public:
	CoreException(String msg, uint8_t errorCode){
		_errorCode = errorCode;
		errorMessage = msg;
		errorMessage += "\n";
	}
	String what(void){
		return errorMessage;
	}
	uint8_t errorCode(void){
		return _errorCode;
	}
	
	void out(void){
		Serial.printf("[%d] %s", this->errorCode(), this->what().c_str());
	}

  void halt(void){
    while(1){
      this->out();
      delay(1000);
    }
  }
};
