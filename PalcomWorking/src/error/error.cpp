#include <Arduino.h>
#include <cstdint>
#include "./error.h"

CoreException::CoreException(String msg, uint8_t errorCode){
	_errorCode = errorCode;
	errorMessage = msg;
	errorMessage += "\n";
}

String CoreException::what(void){
	return errorMessage;
}

uint32_t CoreException::errorCode(void){
	return _errorCode;
}

const char *CoreException::codeTranslate(void){
	switch(this->errorCode()){
		case CORE_ERR_TASK:
			return "CORE_ERR_TASK";
		case ERR_TASK_CREATE:
			return "ERR_TASK_CREATE";
		case ERR_TASK_SEMAPHORE:
			return "ERR_TASK_SEMAPHORE";
		case ERR_TASK_NULL:
			return "ERR_TASK_NULL";
		case CORE_ERR_TQ:
			return "CORE_ERR_TQ";
		case ERR_TQ_SEMAPHORE:
			return "ERR_TQ_SEMAPHORE";
		case ERR_TQ_POP:
			return "ERR_TQ_POP";
		case ERR_TQ_PUSH:
			return "ERR_TQ_PUSH";
	}
	return "ERR_UNKNOWN";
}
	
void CoreException::out(void){
	Serial.printf("[%s] %s", this->codeTranslate(), this->what().c_str());
}

void CoreException::halt(void){
	while(1){
		this->out();
		delay(1000);
	}
}

