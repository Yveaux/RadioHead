// RHEncryptedDriver.cpp
//
// Author: Philippe.Rochat'at'gmail.com

#include <RHEncryptedDriver.h>

RHEncryptedDriver::RHEncryptedDriver(RHGenericDriver& driver, BlockCipher& blockcipher) : _driver(driver), _blockcipher(blockcipher) {
	buffer = calloc(_driver.maxMessageLength(), sizeof(uint8_t));
}

// Just a passthru method
bool RHEncryptedDriver::available() {
	return _driver.available();
}

bool RHEncryptedDriver::recv(uint8_t* buf, uint8_t* len) {
	bool status = _driver.recv(buf, len);

	if(status) {
		int blockSize = _blockcipher.blockSize(); // Size of blocks used by encryption
		int nbBlocks = *len / blockSize; 		  // Number of blocks in that message

		if(nbBlocks * blockSize == *len) { // Or we have a missmatch ... this is probably not symetrically encrypted 
			// Copy message into buffer
			for(int i=0; i < *len; i++) {
				buffer[i] = buf[i];
			}
			for(int k=0; k<nbBlocks; k++) { // Decrypt each block
				_blockcipher.decryptBlock(&buf[k*blockSize],&buffer[k*blockSize]); // Decrypt that block into buf				
			}
		}
	}

	return status;
}

bool RHEncryptedDriver::send(const uint8_t* data, uint8_t len) {
	bool status = true;
	int blockSize = _blockcipher.blockSize(); // Size of blocks used by encryption
	
	if(len == 0) // PashThru
		return _driver.send(data, len);
	if(cipheringBlocks.blockSize == 0) { // We have to allocate blocks
		cipheringBlocks.inputBlock = calloc(blockSize, sizeof(uint8_t));
		cipheringBlocks.blockSize = blockSize;
	}
	if(cipheringBlocks.blockSize != blockSize) { // Cipher has changed it's block size
		cipheringBlocks.inputBlock = realloc(cipheringBlocks.inputBlock, blockSize);
		cipheringBlocks.blockSize = blockSize;	
	}
	uint8_t nbBlocks = (len-1) / blockSize + 1; // How many blocks do we need for that message
	uint8_t nbMsg = (nbBlocks * blockSize) / _driver.maxMessageLength() + 1; // How many message do we need
	uint8_t nbBpM = _driver.maxMessageLength() / blockSize; // Max number of blocks per message

	for(int i=0; i<nbMsg; i++) { // i message
		int k;
		for(k=0; k < nbBpM && k * blockSize < len ; k++) { // k blocks in that message
			for(int j = (i*nbMsg*nbBpM*blockSize+k*blockSize), h=0; j<(i*nbMsg*nbBpM*blockSize+k*blockSize)+blockSize; j++, h++) {
				// Copy each msg byte into inputBlock, and trail with 0 if necessary
				if(j<len) {
					cipheringBlocks.inputBlock[h] = data[j];
				} else {
					cipheringBlocks.inputBlock[h] = 0;
				}
			}
			_blockcipher.encryptBlock(&buffer[k*blockSize], cipheringBlocks.inputBlock); // Cipher that message into buffer
		}
		if(! _driver.send(buffer, k*blockSize)) // We now send that message with it's new length
		 	status = false;
	}
	return status;
}

// Just a passthru method
uint8_t RHEncryptedDriver::maxMessageLength() {
	return _driver.maxMessageLength();
}

