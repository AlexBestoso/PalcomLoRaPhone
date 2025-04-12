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
};
