class PalcomAes{
	private:
		mbedtls_ctr_drbg_context ctr_drbg;
		mbedtls_entropy_context entropy;
		char *pers = "Palcom AES";

	public:
		unsigned char key[32];
		unsigned char iv[16];

		void clearKey(void){
			for(int i=0; i<32; i++)
				key[i] = 0x0;
		}

		void clearIv(void){
			for(int i=0; i<16; i++)
				iv[i] = 0x0;
		}

		void free(void){
                        mbedtls_ctr_drbg_free(&ctr_drbg);
                        mbedtls_entropy_free(&entropy);
                }
		bool generate(void){
			mbedtls_entropy_init(&entropy);
			mbedtls_ctr_drbg_init(&ctr_drbg);

			int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (unsigned char *)pers, strlen(pers));
			if(ret != 0){
				Serial.printf("Failed to seed byte generator.\n");
				this->free();
				return false;
			}

			ret = mbedtls_ctr_drbg_random(&ctr_drbg, key, 32);
			if(ret != 0){
				Serial.printf("Failed to generate random bytes for key.\n");
				this->free();
				return false;
			}

			ret = mbedtls_ctr_drbg_random(&ctr_drbg, iv, 16);
			if(ret != 0){
				Serial.printf("Failed to generate random bytes for iv.\n");
				this->free();
				return false;
			}

			this->free();
			return true;
		}
};
