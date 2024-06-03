struct PalcomAuthData{
	uint8_t paranoia_mode=0;
	uint8_t fail_count=0;
	uint8_t pin_hash[32];
	uint8_t aes_key[32];
	uint8_t aes_iv[16];
	uint8_t phone_number[11];
};
typedef struct PalcomAuthData palcom_auth_t;
