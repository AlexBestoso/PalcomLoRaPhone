#include <src/partition/partition.h>
class PalcomSettings{
	private:
		PalcomPartition partition;
		palcom_partition_t data;
		int mode;
	public:
		PalcomSettings(void);
		~PalcomSettings();

		void load(void);
		void update(void);
	
		palcom_partition_t getPartition(bool);
		void setEncryption(const char *);
		void setEncoding(const char *);
		void setSendRoute(const char *);
		void setRecvRoute(const char *);
		void setPrepend(const char *);
		void setAppend(const char *);
};
