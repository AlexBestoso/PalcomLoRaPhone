class PalcomAes{
	private:
		mbedtls_aes_context aes;
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

		size_t calcOutputSize(size_t inputSize){
			return (inputSize % 16) == 0 ? inputSize : inputSize + (16 - (inputSize % 16));
		}
		void crypt(bool encrypt, unsigned char *input, size_t inputSize, unsigned char *output, size_t outputSize){
			if(encrypt)
				mbedtls_aes_setkey_enc(&aes, (const unsigned char *)this->key, 256);
			else
				mbedtls_aes_setkey_dec(&aes, (const unsigned char *)this->key, 256);

			mbedtls_aes_crypt_cbc(&aes, encrypt ? MBEDTLS_AES_ENCRYPT : MBEDTLS_AES_DECRYPT, outputSize, this->iv, input, output);

		}

		bool testEncDec(){
			this->fetchMasterKey();
			PalcomFS pfs;
			pfs.clearFileBuffer();
			pfs.clearCompBuffer();
			
			String plain = "Palcom AES Test.\n";
			for(int i=0; i<plain.length(); i++){
				fileData[i] = plain[i];
			}

			size_t cipherTextSize = this->calcOutputSize(plain.length());
			Serial.printf("Plaintext size : %ld\nCalculated ciphertext size: %ld\n", plain.length(), cipherTextSize);
			this->crypt(true, (unsigned char *)&fileData, plain.length(), (unsigned char *)&compBuffer, cipherTextSize);
			String cipher = (const char *)compBuffer;

			this->fetchMasterKey();
			pfs.clearFileBuffer();
			pfs.clearCompBuffer();
			for(int i=0; i<cipherTextSize; i++){
				fileData[i] = cipher[i];
			}
			
			size_t plaintextSize = this->calcOutputSize(cipherTextSize);
			this->crypt(false, (unsigned char *)&fileData, cipherTextSize, (unsigned char *)&compBuffer, plaintextSize);
			String result = (const char *)compBuffer;
			Serial.printf("Result Size: %ld\n", result.length());

			Serial.printf("Results:\n\t1) %s\n\t2) ", plain.c_str());
			for(int i=0; i<cipherTextSize;i++){
				Serial.printf("0x%x ", cipher[i]);
			}Serial.printf("\n\t3) %s\n", result.c_str());

			if(result.length() != plain.length()){
				Serial.printf("Result length doesn't match original.\n");
				return false;
			}
			for(int i=0; i<result.length(); i++){
				if(result[i] != plain[i]){
					Serial.printf("%x != %x\n", result[i], plain[i]);
					return false;
				}
			}
			Serial.printf("Victory!\n");
			return true;

		}

		bool fetchMasterKey(void){
			PalcomPartition pp;
			pp.fetchPartitionByName("app1");
			palcom_auth_t auth;
			pp.readAuthData((const esp_partition_t *)pp.partition, &auth);
			pp.freePartitions();
			for(int i=0; i<32; i++){
				this->key[i] = auth.aes_key[i];
			}

			for(int i=0; i<16; i++){
				this->iv[i] = auth.aes_iv[i];
			}

			return true;
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
