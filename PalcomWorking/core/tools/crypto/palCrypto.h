/*
 * Required includes
 * */
#include "mbedtls/config.h"
#include "mbedtls/md.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/bignum.h"
#include "mbedtls/x509.h"
#include "mbedtls/rsa.h"
#include "mbedtls/aes.h"

#include "base64.h"
/*
 * Useful defines
 * */
#define RSA_KEY_SIZE 4096
#define RSA_EXPONENT 65537
#define RSA_KEY_SIZE_BYTES RSA_KEY_SIZE/8

#define KEYS_DIR "/myKeys"

/*
 * Custom Classes
 * */
#include "./palcomAes.h"
#include "./palcomRsaKeyGen.h"
#include "./palcomHash.h"

class PalcomCrypto{
	private:
		
	public:
		PalcomRsaKeyGen rsaKeyGen;
		PalcomHash hash;
		PalcomAes aes;

		void generatePublicHash(bool regen=false){
  			if(!SD.exists(pfs_file_keysPublic)){
    				throw CoreException("No Public Key Detected.", 0x00);
  			}

  			if(SD.exists(pfs_file_publicHash)){
    				if(!regen)
     					throw CoreException("Not regenerating hash.", 0x00);
    				SD.remove(pfs_file_publicHash);
  			}
	
	  		for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++){
	    			fileData[i] = 0;
	  		}

	  		File keyF = SD.open(pfs_file_keysPublic, FILE_READ);
	  		keyF.read(fileData, keyF.size());
	  		size_t keySize = keyF.size();
	  		keyF.close();

			PalcomCrypto pcry;
			pcry.hash.useSHA256();
			pcry.hash.run((unsigned char *)fileData, keySize);

	  		File hashF = SD.open(pfs_file_publicHash, FILE_WRITE);
	  		hashF.write((const uint8_t *)pcry.hash.getResult().c_str(), pcry.hash.getResultSize());
	  		hashF.close();
		}


};

byte shaResult[32];
void getSha256Hash(char *payload, const size_t payloadLength, char* ret){
  	mbedtls_md_context_t ctx;
  	mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
  
  	mbedtls_md_init(&ctx);
  	mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  	mbedtls_md_starts(&ctx);
  	mbedtls_md_update(&ctx, (const unsigned char *) payload, payloadLength);
  	mbedtls_md_finish(&ctx, shaResult);
  	mbedtls_md_free(&ctx);
  	string grabber = "";
  	for(int i= 0; i< sizeof(shaResult); i++){
      		char str[3];
 
      		sprintf(str, "%02x", (int)shaResult[i]);
      		// TODO: Replace this grabber line to prevent memory fragmentation.
      		grabber += str;
  	}
  
	for(int i=0; i<32; i++){
    		ret[i] = grabber[i];
  	}
  	ret[32] = 0x00;
}

bool rsaDecrypt(const unsigned char *buf, size_t bufSize, const char *outLoc){
  	int ret;
  	mbedtls_rsa_context rsa;
  	mbedtls_entropy_context entropy;
  	mbedtls_ctr_drbg_context ctr_drbg;
  	mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
  	const char *pers = "rsa_decrypt";
  	File outFile;
  	File keyFile;
  	char *keyPointer;
  	int pointerOffset = 0;

  	for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
    		fileData[i] = 0x00;

  	Serial.printf("RSA DECRYPT : 0\n");
  	keyFile = SD.open(pfs_file_keysPrivate, FILE_READ);
  	if(!keyFile)
    		return false;
  	size_t fileSize = keyFile.size();
  	// ToDo: Handle buffer overflows.
  	keyFile.read(fileData, fileSize);
  	keyFile.close();

  	mbedtls_ctr_drbg_init(&ctr_drbg);
  	mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
  	mbedtls_mpi_init( &N ); mbedtls_mpi_init( &P ); mbedtls_mpi_init( &Q );
  	mbedtls_mpi_init( &D ); mbedtls_mpi_init( &E ); mbedtls_mpi_init( &DP );
  	mbedtls_mpi_init( &DQ ); mbedtls_mpi_init( &QP );

  	mbedtls_entropy_init(&entropy);

  	if((ret = mbedtls_ctr_drbg_seed(
		&ctr_drbg, 
      		mbedtls_entropy_func, 
      		&entropy, 
      		(const unsigned char *) pers, 
      		strlen(pers)
	)) != 0 ){
      		Serial.printf("Failed to seed PRNG : %d\n", ret );
      		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
      		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
      		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
      		mbedtls_rsa_free( &rsa );
      		mbedtls_ctr_drbg_free( &ctr_drbg );
      		mbedtls_entropy_free( &entropy );
      		return false;
  	}


  	keyPointer = (char *)&fileData[pointerOffset];
  	if((ret = mbedtls_mpi_read_string( &N, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &P, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &Q, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &D, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &E, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if((ret = mbedtls_mpi_read_string( &DP, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &DQ, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}else{
    		pointerOffset += strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}
  

  	if( (ret = mbedtls_mpi_read_string( &QP, 16, (const char *)fileData ) ) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to read string.\n");
    		return false;
  	}


  	if(mbedtls_rsa_import(&rsa, &N, &P, &Q, &D, &E) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to import.\n");
    		return false;
  	}


 	if((ret = mbedtls_rsa_complete(&rsa)) != 0){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Failed to complete. : %d\n", ret);
    		return false;
  	}

  	size_t rsasize = rsa.len;
  	for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
    		fileData[i] = 0x00;
  
  	if((ret = mbedtls_rsa_pkcs1_decrypt(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PRIVATE, &rsasize, buf, fileData, RSA_KEY_SIZE))){
    		mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
    		mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
    		mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Direct decryption error. : %d\n", ret);
    		char error_buf[200];
    		mbedtls_strerror( ret, error_buf, 200 );
    		Serial.printf("Last error was: -0x%04x - %s\n\n", (int) -ret, error_buf );
    		return false;
  	}

  	outFile = SD.open(outLoc, FILE_WRITE);
  	outFile.write(fileData, rsa.len);
  	outFile.close();
  	mbedtls_mpi_free( &N ); mbedtls_mpi_free( &P ); mbedtls_mpi_free( &Q );
  	mbedtls_mpi_free( &D ); mbedtls_mpi_free( &E ); mbedtls_mpi_free( &DP );
  	mbedtls_mpi_free( &DQ ); mbedtls_mpi_free( &QP );
  	mbedtls_rsa_free( &rsa );
  	mbedtls_ctr_drbg_free( &ctr_drbg );
  	mbedtls_entropy_free( &entropy );
  	return true;
}

bool rsaEncrypt(const char *keyLoc, const unsigned char *buf, size_t bufSize, const char *outLoc){
  	if(bufSize <= 0 || !outLoc || !buf || !keyLoc){
    		return false;
  	}
  
  	int ret;
  	mbedtls_rsa_context rsa;
  	mbedtls_entropy_context entropy;
  	mbedtls_ctr_drbg_context ctr_drbg;
  	mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
  	const char *pers = "rsa_genkey";
  	File outFile;
  	File keyFile;
  	char *keyPointer;
  	int pointerOffset = 0;

  	if(!SD.exists(keyLoc)){
    		return false;
  	}
  
	for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
    		fileData[i] = 0x00;

  	keyFile = SD.open(keyLoc, FILE_READ);
  	if(!keyFile)
    		return false;
  
  	size_t fileSize = keyFile.size();
  	// ToDo: Handle buffer overflows.
  	keyFile.read(fileData, fileSize);
  	keyFile.close();

  	mbedtls_ctr_drbg_init(&ctr_drbg);
  	mbedtls_rsa_init( &rsa, MBEDTLS_RSA_PKCS_V15, 0 );
  	mbedtls_mpi_init( &N ); 
  	mbedtls_mpi_init( &E );

  	mbedtls_entropy_init(&entropy);

  	if((ret = mbedtls_ctr_drbg_seed(
      		&ctr_drbg, 
      		mbedtls_entropy_func, 
      		&entropy, 
      		(const unsigned char *) pers, 
      		strlen( pers ) 
    	)) != 0 ){
      		Serial.printf("Failed to seed PRNG : %d\n", ret );
      		mbedtls_mpi_free( &N );
      		mbedtls_mpi_free( &E );
      		mbedtls_rsa_free( &rsa );
      		mbedtls_ctr_drbg_free( &ctr_drbg );
      		mbedtls_entropy_free( &entropy );
      		return false;
  	}


  	pointerOffset = 0;
  	keyPointer = (char *)&fileData[pointerOffset];
  	if((ret = mbedtls_mpi_read_string( &N, 16, (const char *)keyPointer ) ) != 0){
    		mbedtls_mpi_free( &N ); 
    		mbedtls_mpi_free( &E ); 
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		return false;
  	}else{
    		pointerOffset = strlen(keyPointer)+1;
    		keyPointer = (char *)&fileData[pointerOffset];
  	}


  	if(( ret = mbedtls_mpi_read_string( &E, 16, (const char *)keyPointer ) ) != 0 ){  
    		mbedtls_mpi_free( &N ); 
    		mbedtls_mpi_free( &E ); 
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("String read failed with code %d|%x\n", ret, ret);
    		return false;
  	}


  	if(mbedtls_rsa_import(&rsa, &N, NULL, NULL, NULL, &E) != 0){
    		mbedtls_mpi_free( &N );
    		mbedtls_mpi_free( &E );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		return false;
  	}


  	for(int i=0; i<__GLOBAL_BUFFER_SIZE; i++)
    		fileData[i] = 0x00;

  	if(mbedtls_rsa_pkcs1_encrypt(&rsa, mbedtls_ctr_drbg_random, &ctr_drbg, MBEDTLS_RSA_PUBLIC, bufSize, buf, fileData) != 0){
    		mbedtls_mpi_free( &N );
    		mbedtls_mpi_free( &E );
    		mbedtls_rsa_free( &rsa );
    		mbedtls_ctr_drbg_free( &ctr_drbg );
    		mbedtls_entropy_free( &entropy );
    		Serial.printf("Direct decryption error. : %d\n", ret);
    		char error_buf[200];
    		mbedtls_strerror( ret, error_buf, 200 );
    		Serial.printf("Last error was: -0x%04x - %s\n\n", (int) -ret, error_buf );
    		return false;
  	}

  	outFile = SD.open(outLoc, FILE_WRITE);
  	outFile.write(fileData, rsa.len);
  	outFile.close();

  	mbedtls_mpi_free( &N );
  	mbedtls_mpi_free( &E );
  	mbedtls_rsa_free( &rsa );
  	mbedtls_ctr_drbg_free( &ctr_drbg );
  	mbedtls_entropy_free( &entropy );
  	return true;
}
