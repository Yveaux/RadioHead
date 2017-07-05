// RHEncryptedDriver.cpp
//
// Author: Philippe.Rochat'at'gmail.com

#include <RHEncryptedDriver.h>

RHEncryptedDriver::RHEncryptedDriver(RHGenericDriver& driver) : _driver(driver) {
	
}

bool RHEncryptedDriver::available() {
	return _driver.available();
}

bool RHEncryptedDriver::recv(uint8_t* buf, uint8_t* len) {
	return _driver.recv(buf, len);
}

bool RHEncryptedDriver::send(const uint8_t* data, uint8_t len) {
	return _driver.send(data, len);
}

uint8_t RHEncryptedDriver::maxMessageLength() {
	return _driver.maxMessageLength();
}

