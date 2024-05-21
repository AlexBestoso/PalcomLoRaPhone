struct PalcomAuthData{
	uint8_t paranoia_mode;
	uint8_t pin_hash[32];
	uint8_t aes_key[32];
	uint8_t aes_iv[16];
};
typedef struct PalcomAuthData palcom_auth_t;
