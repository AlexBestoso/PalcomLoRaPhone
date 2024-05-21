const char *pfs_dir_tmp = "/tmp";
const char *pfs_dir_friends = "/friends";
const char *pfs_file_friendsList = "/friends/friends.list";
const char *pfs_dir_keys = "/keys";
const char *pfs_file_keysPublic = "/keys/pub.key";
const char *pfs_file_publicHash = "/keys/pub.hash";
const char *pfs_file_keysPrivate = "/keys/pri.key";
const char *pfs_file_callsign = "/keys/callsign";
const char *pfs_dir_public = "/public";
const char *pfs_file_publicLog= "/public/msgLog";
const char *pfs_dir_requests = "/requests";
const char *pfs_public_msg_log = "/public/msgLog";
const char *pfs_public_msg_dir = "/public";
const char *pfs_file_cryptSend  = "/cryptSend.enc";
const char *pfs_file_cryptRecv  = "/cryptRecv.enc";
const char *pfs_folder_sendQueue = "/sendQueue";
const char *pfs_folder_recvQueue = "/recvQueue";
const char *pfs_error_log = "/error.log";
const char *pfs_config = "/settings.conf";

#define __GLOBAL_BUFFER_SIZE 100000
unsigned char fileData[__GLOBAL_BUFFER_SIZE] = {0};
char compBuffer[__GLOBAL_BUFFER_SIZE] = {0};
char fileNameBuffer[128];

class PalcomFS{
  	private:
  
  	public:
  		File fd;

  		size_t getPublicMessages(){
    			clearFileBuffer();
    			fd = SD.open(pfs_file_publicLog, FILE_READ);
    			fd.read(fileData, fd.size());
    			size_t ret = fd.size();
    			fd.close();
    			return ret;
  		}

  		size_t getFriendMessages(string friendHash){
    			clearFileBuffer();
    			sprintf((char *)compBuffer, "%s/%s/msgLog", pfs_dir_friends, friendHash.c_str());
    			Serial.printf("Loading messages from file '%s'", compBuffer);
    			fd = SD.open(compBuffer, FILE_READ);
    			fd.read(fileData, fd.size());
    			size_t ret = fd.size();
    			fd.close();
    			return ret;
  		}

  		void deleteRootKeyPair(void){
    			if(SD.exists(pfs_file_keysPublic))
      				SD.remove(pfs_file_keysPublic);
    
    			if(SD.exists(pfs_file_keysPrivate))
      				SD.remove(pfs_file_keysPrivate);
  		}

  		void createKeysFolder(void){
    			if(!SD.exists(pfs_dir_keys)){
      				SD.mkdir(pfs_dir_keys);
    			}
  		}

  		void rm(const char *target){
    			if(!SD.exists(target))
      				return;
      			File root = SD.open(target);
      			if(root.isDirectory()){
        			while(true){
          				File node = root.openNextFile();
          				if(!node)
            					break;
          				string name;
          				name = target;
          				if(name != "/")
            					name += "/";
          				name += node.name();
          				if(node.isDirectory()){
            					this->rm(name.c_str());
            					node.close();
            					if(!SD.rmdir(name.c_str()))
              						Serial.printf("Failed to remove directory %s\n", name.c_str());
          				}else{
            					node.close();
            					SD.remove(name.c_str());
          				}
        			}
        			root.close();
        			if(!SD.rmdir(target))
          				Serial.printf("Failed to remove directory %s\n", (const char *)target);
      			}else{
        			root.close();
        			SD.remove(target);
      			}
  		}

  		const char *getCallsign(){
    			File target = SD.open(pfs_file_callsign, FILE_READ);
    			if(!target)
      				return NULL;
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      				fileData[i] = 0;
    			target.read(fileData, target.size());
    			target.close();
    			return (const char *)fileData;
  		}

  		void setCallsign(const char *val){
    			File target = SD.open(pfs_file_callsign, FILE_WRITE, O_TRUNC);
    			if(!target)
      				return;
    			target.write((const uint8_t *)val, strlen(val));
    			target.close();
  		}

  		const char *getFilenameByPos(int id, const char *targetDir){
  			if(SD.exists(targetDir)){
      				int buttonCount = 0;
      				File root = SD.open(targetDir);
      				while(true){
        				if(buttonCount > 25)
        					break;
        				File node = root.openNextFile();
        				if(!node)
          					break;
        				if(buttonCount == id){
          					const char *ret = node.name();
          					node.close();
          					root.close();
          					return ret;
        				}
        				node.close(); 
      				}
      				root.close();
    			}
    			return NULL;
  		}

  		bool getPublicHash(void){
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
      				fileData[i] = 0;
    			}
    			File hash = SD.open(pfs_file_publicHash, FILE_READ);
    			if(!hash)
      				return false;

      			hash.read(fileData, hash.size());
      			hash.close();
      			return true;
  		}

  		bool getPublicHash(char *ret){
    			File hash = SD.open(pfs_file_publicHash, FILE_READ);
    			if(!hash)
      				return false;

      			hash.read((uint8_t *)ret, hash.size());
      			hash.close();
      			return true;
  		}

  		int rootKeysExist(void){
    			if(SD.exists(pfs_file_keysPublic) && SD.exists(pfs_file_keysPrivate))
      				return 1;
    			else if(SD.exists(pfs_file_keysPublic) && !SD.exists(pfs_file_keysPrivate))
      				return 2;
    			else if(!SD.exists(pfs_file_keysPublic) && SD.exists(pfs_file_keysPrivate))
      				return 3;
    			return 0;
  		}

  		bool openPublicKey(const char *mode){
  			fd = SD.open(pfs_file_keysPublic, mode);
    			return (fd) ? true : false;
  		}

  		bool openPrivateKey(const char *mode){
    			fd = SD.open(pfs_file_keysPrivate, mode);
    			return (fd) ? true : false;
  		}

  		void close(void){
    			if(fd)
      				fd.close();
  		}

  		void write(unsigned char *buf, size_t size){
    			fd.write(buf, size);
  		}

  		void clearFileBuffer(void){
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      				fileData[i] = 0;
  		}

  		void clearCompBuffer(void){
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
      				compBuffer[i] = 0;
  		}

  		void clearAllBuffers(void){
    			for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
      				compBuffer[i] = 0;
      				fileData[i] = 0;
    			}
  		}

  		void addToFiledata(char *buf, size_t bufSize){
    			for(int i=0; i<bufSize && i<__GLOBAL_BUFFER_SIZE; i++){
      				if(i < bufSize){
        				fileData[i] = buf[i];
      				}else{
        				fileData[i] = 0;
      				}
    			}
  		}

  		bool popSendQueue(void){
    			lv_task_handler();
    			if(!SD.exists(pfs_folder_sendQueue)){
      				lv_task_handler();
      				return false;
    			}

    			File root = SD.open(pfs_folder_sendQueue);
    			lv_task_handler();
    			File node = root.openNextFile();
    			lv_task_handler();
    			if(!node){
      				node.close();
      				root.close();
      				return false;
    			}

    			sprintf((char *)fileNameBuffer, "%s/%s", pfs_folder_sendQueue, node.name());
    			node.close();
    			root.close();
    			fd = SD.open((const char *)fileNameBuffer, FILE_READ);
    			lv_task_handler();
    			return true;
  		}

  		size_t fileAppend(const char *target, uint8_t * buf, size_t bufSize){
    			File targetFile;
    			size_t targetSize = 0;
    			clearFileBuffer();

    			if(!SD.exists(target)){
      				targetFile = SD.open(target, FILE_WRITE);
    			}else{
      				targetFile = SD.open(target, FILE_READ);
      				targetSize = targetFile.size();
      				targetFile.read(fileData, targetSize);
      				targetFile.close();
      				targetFile = SD.open(target, FILE_WRITE, O_TRUNC);
    			}

    			if(targetSize > 0)
      				targetFile.write(fileData, targetSize);
    
    			targetFile.write(buf, bufSize);
    			targetFile.close();
    			targetFile = SD.open(target, FILE_READ);
    			targetSize = targetFile.size();
    			targetFile.close();

    			return targetSize;
  		}

  		bool validFriend(const char *friendHash){
    			sprintf(fileNameBuffer, "%s/%s", pfs_dir_friends, friendHash);
    			if(!SD.exists(fileNameBuffer))
      				return false;
    			return true;
  		}

		void storeConfigData(palcom_config_t data){
			this->fd = SD.open(pfs_config, FILE_WRITE, O_TRUNC);
			this->fd.write((unsigned char *)&data, sizeof(palcom_config_t));
			this->close();
		}
};
