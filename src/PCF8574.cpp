/*
 * PCF8574.cpp
 *
 *  Created on: 10 apr. 2017
 *      Author: michi_000
 */

#include "PCF8574.h"

#include <Wire.h>

PCF8574::PCF8574(int address) {
	_address = address;
	Wire.begin();
}

uint8_t PCF8574::digitalRead() {
	Wire.beginTransmission(_address);
	Wire.requestFrom(_address, 1);
	_data = Wire.read();
	_error = Wire.endTransmission();
	return _data;
}

uint8_t PCF8574::value() {
	return _data;
}

int PCF8574::digitalWrite(uint8_t value) {
	Serial.println("PCF8574::digitalWrite" + String(value));
	Wire.beginTransmission(_address);
	_data = value;
	Wire.write(~_data);
	_error = Wire.endTransmission();
	return _error;
}

uint8_t PCF8574::digitalRead(uint8_t pin) {
	PCF8574::digitalRead();
	return (_data & (1 << pin)) > 0;
}

void PCF8574::digitalWrite(uint8_t pin, uint8_t value) {
	PCF8574::digitalRead();
	if (value == LOW) {
		_data &= ~(1 << pin);
	} else {
		_data |= (1 << pin);
	}
	PCF8574::digitalWrite(~_data);
}

void PCF8574::toggle(uint8_t pin) {
	PCF8574::digitalRead();
	_data ^= (1 << pin);
	PCF8574::digitalWrite(~_data);
}

void PCF8574::shiftRight(uint8_t n) {
	if (n == 0 || n > 7)
		return;
	PCF8574::digitalRead();
	_data >>= n;
	PCF8574::digitalWrite(~_data);
}

void PCF8574::shiftLeft(uint8_t n) {
	if (n == 0 || n > 7)
		return;
	PCF8574::digitalRead();
	_data <<= n;
	PCF8574::digitalWrite(~_data);
}

int PCF8574::lastError() {
	int e = _error;
	_error = 0;
	return e;
}
