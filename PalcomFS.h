const char *pfs_dir_tmp = "/tmp";
const char *pfs_dir_friends = "/friends";
const char *pfs_file_friendsList = "/friends/friends.list";
const char *pfs_dir_keys = "/keys";
const char *pfs_file_keysPublic = "/keys/pub.key";
const char *pfs_file_keysPrivate = "/keys/pri.key";
const char *pfs_dir_public = "/public";
const char *pfs_file_publicLog= "/public/msgLog";
const char *pfs_dir_requests = "/requests";

#define __GLOBAL_BUFFER_SIZE 100000
unsigned char fileData[__GLOBAL_BUFFER_SIZE] = {0};

class PalcomFS{
  private:
  char *fileNameBuffer[128];
  
  public:
  File fd;

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
};