struct palcom_partition{
	uint8_t mode = 0; // general phone mode
	uint8_t encryption = 0; // Encryption settings
	uint8_t encoding = 0;	// encoding settings
	uint8_t send_route = 0; // what to send msgs through
	uint8_t recv_route = 0;	// what to recv msgs through
	uint8_t pre[8]; // Prepended text
	uint8_t app[8]; // Appended text
	
};
typedef struct palcom_partition palcom_partition_t;

class PalcomPartition{
	private:
		esp_partition_iterator_t _partitionIterator = NULL;
		const char *defualtPartition = "app1";
	public:
		esp_partition_t *partition = NULL;

		bool getAllPartitions(void);
		void freePartitions(void);
		bool fetchPartition(void);
		bool eraseRange(const esp_partition_t *partition, size_t offset, size_t eraseRange);
		bool readPartition(const esp_partition_t *partition, size_t offset, void *outBuf, size_t outBufSize);
		bool writePartition(const esp_partition_t *partition, size_t offset, const void *src, size_t srcSize);
		bool writeData(const esp_partition_t *partition, palcom_partition_t data);
		bool write(palcom_partition_t data);
		bool read(palcom_partition_t *ret);
		bool fetchPartitionByName(String name);
		bool readData(const esp_partition_t *partition, palcom_partition_t *ret);
};
