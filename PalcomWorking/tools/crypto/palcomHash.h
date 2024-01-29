class PalcomHash{
	private:
		mbedtls_md_context_t ctx;
  		mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

		byte hashResult[64];
		size_t hashSize = 0;
		string resultString = "";
	public:
		void clearResult(void){
			for(int i=0; i<64; i++)
				hashResult[i] = 0x0;
			hashSize = 0;
			resultString = "";
		}

		size_t getResultSize(void){
			return hashSize;
		}

		string getResult(void){
			return resultString;
		}
		PalcomHash(){
			this->clearResult();
		}
		
		void useMD2(void){
			md_type = MBEDTLS_MD_MD2;
		}

		void useMD4(void){
        	        md_type = MBEDTLS_MD_MD4;
        	}

		void useMD5(void){
        	        md_type = MBEDTLS_MD_MD5;
        	}

		void useSHA1(void){
        	        md_type = MBEDTLS_MD_SHA1;
			hashSize = 128/8;
        	}

		void useSHA224(void){
        	        md_type = MBEDTLS_MD_SHA224;
			hashSize = 224/8;
        	}

		void useSHA256(void){
        	        md_type = MBEDTLS_MD_SHA256;
			hashSize = 256/8;
        	}

		void useSHA384(void){
        	        md_type = MBEDTLS_MD_SHA384;
			hashSize = 384/8;
        	}

		void useSHA512(void){
        	        md_type = MBEDTLS_MD_SHA512;
			hashSize = 512 / 8;
        	}

		void run(unsigned char *payload, size_t payloadLength){
			mbedtls_md_init(&ctx);
  			mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  			mbedtls_md_starts(&ctx);
  			mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
  			mbedtls_md_finish(&ctx, hashResult);
  			mbedtls_md_free(&ctx);
	
			resultString = "";
			for(int i= 0; i< sizeof(hashResult); i++){
				if(i >= hashSize)
					break;
	      			char str[3];
	      			sprintf(str, "%02x", (int)hashResult[i]);
	      			resultString += str;
	  		}
		}
};
