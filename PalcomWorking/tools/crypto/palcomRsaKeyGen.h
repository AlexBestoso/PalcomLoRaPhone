class PalcomRsaKeyGen{
	private:
	public:
		int ret;
  		size_t keySize = 0;
  		const char *pers = "rsa_genkey";
		
		PalcomFS pfs;
  		
		mbedtls_rsa_context rsa;
  		mbedtls_entropy_context entropy;
  		mbedtls_ctr_drbg_context ctr_drbg;
  		mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
		
		/*
		 * This must run before any other functions.
		 * */
		void initalizeVars(void){
			mbedtls_ctr_drbg_init(&ctr_drbg);
  			mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
  			mbedtls_mpi_init( &N ); mbedtls_mpi_init( &P ); mbedtls_mpi_init( &Q );
  			mbedtls_mpi_init( &D ); mbedtls_mpi_init( &E ); mbedtls_mpi_init( &DP );
  			mbedtls_mpi_init( &DQ ); mbedtls_mpi_init( &QP );
			mbedtls_entropy_init(&entropy);
		}

		/*
		 * This must be called when you're done using the class.
		 * */
		void freeVars(void){
			mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
      			mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
      			mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
      			mbedtls_rsa_free( &rsa );
      			mbedtls_ctr_drbg_free( &ctr_drbg );
      			mbedtls_entropy_free( &entropy );
		}

		/*
		 * generate randome number seed
		 * */
		bool generateRngSeed(void){
			if((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, 
							mbedtls_entropy_func, 
							&entropy, 
							(const unsigned char *) 
							pers, 
							strlen(pers)
						       )
			) != 0 ){
				this->freeVars();
      				return false;
  			}
			return true;	
		}

		/*
		 * Generate the key pair
		 * */
		bool generateKeyPair(void){
			if((ret = mbedtls_rsa_gen_key(&rsa,
      						      mbedtls_ctr_drbg_random,
      						      &ctr_drbg,
      						      RSA_KEY_SIZE,
      						      RSA_EXPONENT
    						     )
			) != 0){
				this->freeVars();
      				return false;
  			}

			return true;
		}

		/*
		 * Export keys out from the encryption accelerator.
		 * */
		bool exportKeys(){
			if( (ret = mbedtls_rsa_export(&rsa, &N, &P, &Q, &D, &E)) != 0 || 
			    (ret = mbedtls_rsa_export_crt(&rsa, &DP, &DQ, &QP)) != 0 
			){
				this->freeVars();
        			return false;
  			}
			return true;
		}

		/*
		 * Write the public key to a file for future use.
		 * */
		bool storeExportedPublicKey(void){
			// Open file.
			pfs.clearFileBuffer();
  			if(!pfs.openPublicKey(FILE_WRITE)){
				this->freeVars();
				pfs.close();
    				return false;
  			}
			
			// Process the N variable.
			if((ret = mbedtls_mpi_write_string(&N, 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
    				pfs.close();
				this->freeVars();
				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;
  			

			// Process E variable.
			if((ret = mbedtls_mpi_write_string(&E, 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0 ){
				this->freeVars();
    				pfs.close();
    				return false;
			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			pfs.close();
			return true;
		}

		/*
		 * Write the private key to a file for future use.
		 * */
		bool storeExportedPrivateKey(void){
			if(!pfs.openPrivateKey(FILE_WRITE)){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
			
			// Store the N variable.
			if((ret = mbedtls_mpi_write_string(&N , 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the P variable.
			if((ret = mbedtls_mpi_write_string(&P , 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
   	 		pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the Q variable.			
			if((ret = mbedtls_mpi_write_string(&Q , 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}	
			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the D variable.
			if((ret = mbedtls_mpi_write_string(&D , 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the E variable.			
			if((ret = mbedtls_mpi_write_string(&E , 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the DP variable.
			if((ret = mbedtls_mpi_write_string(&DP, 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
  			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the DQ variable.
			if((ret = mbedtls_mpi_write_string(&DQ, 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			// Store the QP variable.
			if((ret = mbedtls_mpi_write_string(&QP, 16, (char *)fileData, __GLOBAL_BUFFER_SIZE, &keySize)) != 0){
				this->freeVars();
    				pfs.close();
    				return false;
  			}
    			pfs.write(fileData, keySize);
    			keySize = 0;

			pfs.close();
			return true;
		}
		PalcomRsaKeyGen(){
			pfs.createKeysFolder();
		}
		void deleteKeyPair(void){
			if(pfs.rootKeysExist() != 1){
				pfs.deleteRootKeyPair();
			}
		}

		/*
		 * This will delete any existing system key pairs.
		 * */
		bool genKeys(void){
			Serial.printf("Clearing key cache\n");
			this->deleteKeyPair();
			Serial.printf("Initalizing encryption vars.\n");
			this->initalizeVars();
			
			Serial.printf("Generating RNG Seed\n");
			if(!this->generateRngSeed()){
				return false;
			}

			Serial.printf("Generating key pair.\n");
			if(!this->generateKeyPair()){
				return false;		
			}

			Serial.printf("Exporting key pair\n");
			if(!this->exportKeys()){
				return false;
			}

			Serial.printf("Storing Public Key\n");
			if(!this->storeExportedPublicKey()){
				return false;
			}

			Serial.printf("Storing private key\n");
			if(!this->storeExportedPrivateKey()){
				return false;
			}


			Serial.printf("Key Generation Successful!\n");
			this->freeVars();
			return true;
		}
};
