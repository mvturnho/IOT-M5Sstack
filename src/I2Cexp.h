/*
 * I2Cexp.h
 *
 *  Created on: 13 apr. 2017
 *      Author: michi_000
 */

#ifndef I2CEXP_H_
#define I2CEXP_H_

#include <Arduino.h>
#include <Wire.h>
#include <inttypes.h>

#include "BH1750.h"
#include <BME280I2C.h>

#define BH1750_ADDR 	0x23
#define PCA9685_ADDR	0x40
#define BME280_ADDR		0x76
#define PCF8574_ADDR	0x38
#define PCF8575_ADDR	0x20
#define MCP23017_ADDR	0x21 // 0x20 is same as pcf8575 so always connect A0 to 3v

/// I2C/TWI success (transaction was successful).
#define TWI_SUCCESS  	0
/// I2C/TWI device not present (address sent, NACK received).
#define TWI_DEVICE_NACK 2
/// I2C/TWI data not received (data sent, NACK received).
#define TWI_DATA_NACK  	3
/// I2C/TWI other error.
#define TWI_ERROR  		4

class I2Cexp {
public:
	struct Metrics {
		float temp;
		float hum;
		float pres;
		int luxvalue;
	};

	bool haspcf8575 = false;
	bool haspcf8574 = false;
	bool hasBH1750 = false;
	bool hasBME280 = false;
	bool hasPCA9685 = false;
	bool hasMCP23017 = false;

	int nDevices;
	Metrics metrics;

	void initbus(int SDApin, int SCLpin, uint32_t clock = 400000);bool scanaddress(int address);
	int scanbus(void);
	bool getMetrics(void);
	bool hasIOexpander(void);
	bool hasPWM(void);

private:
	BH1750 *lightMeter;
	BME280I2C *bme;
};

#endif /* I2CEXP_H_ */
