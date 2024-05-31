#define PALCOM_CONFIG_DATA_SIZE 22
struct palcomConfigData{
	uint8_t lock_timer=3;
	uint8_t screen_brightness=10;
	uint8_t user_name[20];
};
typedef struct palcomConfigData palcom_config_t;
