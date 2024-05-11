/*
 * Useage : Error code of 0x00 means that the error can be ignored.
 * */
class CoreException : public exception{
	private:
		PalcomFS pfs;
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
	
	void log(String catcher){
		String tmp = "[";
		string tmp2 = to_string(_errorCode);
		for(int i=0; i<tmp2.length(); i++)
			tmp += tmp2[i];
		tmp += "] ";
		tmp += catcher;
		tmp += "() - ";
		tmp += errorMessage;
		pfs.fileAppend(pfs_error_log, (uint8_t*)tmp.c_str(), tmp.length());
	}

};
