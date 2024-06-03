class PalcomAlgorithms{
	private:

	public:

		/*
		 * This algorithm takes the systems's deciphered private key, 
		 * */
		String PalPhoGen7(unsigned char *data, size_t dataSize){
			String ret = "79876501234";
			size_t sectionCount = dataSize / 10; // 10 is the number of digits in a phone number.
			
			/*
			 * Calculate number seeds.
			 * */
			for(int i=10; i<dataSize; i+=20){
				ret[1] ^= (data[i+0] + data[(i-10)+9]);
				ret[2] ^= (data[i+1] - data[(i-10)+8]);
				ret[3] ^= (data[i+2] + data[(i-10)+7]);
				ret[4] ^= (data[i+3] - data[(i-10)+6]);
				ret[5] ^= (data[i+4] + data[(i-10)+5]);
				ret[6] ^= (data[i+5] - data[(i-10)+4]);
				ret[7] ^= (data[i+6] + data[(i-10)+3]);
				ret[8] ^= (data[i+7] - data[(i-10)+2]);
				ret[9] ^= (data[i+8] + data[(i-10)+1]);
				ret[10] ^= (data[i+9] - data[(i-10)+0]);
			}

			/*
			 * Convert seeds into base 10.
			 * */
			for(int i=1; i<11; i++){
				ret[i] = (ret[i] % 10);
			}
			ret[0] = 0x7;
			
			// Enjoy your new number :)
			return ret;
		}
};
