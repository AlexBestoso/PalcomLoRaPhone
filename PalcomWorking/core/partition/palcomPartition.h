class PalcomPartition{
	private:
		esp_partition_iterator_t _partitionIterator = NULL;
	public:
		esp_partition_t *partition = NULL;

		bool getAllPartitions(void){
			this->_partitionIterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
			if(this->_partitionIterator == NULL){
				return false;
			}
			return true;
		}

		void freePartitions(void){
			if(this->_partitionIterator != NULL){
				esp_partition_iterator_release(this->_partitionIterator);
				this->_partitionIterator = NULL;
			}
			if(this->partition != NULL){
				this->partition = NULL;
			}
		}

		bool fetchPartition(void){
			if(this->_partitionIterator == NULL)
				return false;
			this->partition = (esp_partition_t*)esp_partition_get(this->_partitionIterator);
			this->_partitionIterator = esp_partition_next(this->_partitionIterator);
			return true;

		}

		void eraseRange(const esp_partition_t *partition, size_t offset, size_t eraseRange){
			if(partition == NULL)
				throw CoreException("PalcomPartition::eraseRange() - Partition is null", 0x01);
			esp_err_t err = esp_partition_erase_range(partition, offset, eraseRange);
			if(err == ESP_OK){
				return;
			}else if(err == ESP_ERR_INVALID_ARG){
				throw CoreException("PalcomPartition::eraseRange() - Invalid Arguments", 0x02);
			}else if(err == ESP_ERR_INVALID_SIZE){
				throw CoreException("PalcomPartition::eraseRange() - Invalid Size", 0x03);
			}else{
				throw CoreException("PalcomPartition::eraseRange() - Unknown Error Occurred.", 0x04);
			}
		}

		void readPartition(const esp_partition_t *partition, size_t offset, void *outBuf, size_t outBufSize){
			if(partition == NULL)
				throw CoreException("PalcomPartition::readPartition() - Partitition is null.", 0x01);
			
			esp_err_t err = esp_partition_read(partition, offset, outBuf, outBufSize);
			if(err == ESP_OK){
				return;
			}else if(err == ESP_ERR_INVALID_ARG){
				throw CoreException("PalcomPartition::readPartition() - Invaldi esp_partition_read() paramaeters.", 0x02);
			}else if(err == ESP_ERR_INVALID_SIZE){
				throw CoreException("PalcomPartition::readPartition() - Invalid esp_partition_read() size.", 0x03);
			}else{
				throw CoreException("PalcomPartition::readPartition() - Unknown error has occured.", 0x04);
			}
		}

		void writePartition(const esp_partition_t *partition, size_t offset, const void *src, size_t srcSize){
			if(partition == NULL)
				throw CoreException("PalcomPartition::writePartition() - Partitition is null.", 0x01);
			esp_err_t err = esp_partition_write(partition, offset, src, srcSize);
			if(err == ESP_OK){
				return;
			}else if(err == ESP_ERR_INVALID_ARG){
				throw CoreException("PalcomPartition::writePartition() - Partition arguments invalid,", 0x02);
			}else if(err == ESP_ERR_INVALID_SIZE){
				throw CoreException("PalcomPartition::writePartition() - Proviced size is invalid.", 0x03);
			}else{
				throw CoreException("PalcomPartition::writePartition() - An Unknown Error Occured.", 0x04);
			}
		}

		void writeAuthData(const esp_partition_t *partition, palcom_auth_t data){
			Serial.printf("size of auth data : %ld\n", (long)sizeof(palcom_auth_t));
			try{
				this->eraseRange(partition, 0, partition->size);
			}catch(CoreException e){
				String msg = "PalcomPartition::writeAuthData() - Failed to clear flash data.\n\t";
				msg += e.what();
				throw CoreException(msg.c_str(), 0x01);
			}

			Serial.printf("Attempting to store auth data...\n");
			
			try{
				this->writePartition(partition, 0, (const void *)&data, sizeof(palcom_auth_t));
			}catch(CoreException e){
				String msg = "PalcomPartition::writePartition() - Failed to write auth data.\n\t";
				msg += e.what();
				throw CoreException(msg.c_str(), 0x02);
			}

			Serial.printf("Success!\n");
		}
};
