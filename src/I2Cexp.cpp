/*
 * I2Cexp.cpp
 *
 *  Created on: 13 apr. 2017
 *      Author: michi_000
 */

#include "I2Cexp.h"

void I2Cexp::initbus(int SDApin, int SCLpin, uint32_t clock) {
	Wire.begin(SDApin, SCLpin);
	Wire.setClock(clock);

	hasBH1750 = scanaddress(BH1750_ADDR);
	hasBME280 = scanaddress(BME280_ADDR);
	hasPCA9685 = scanaddress(PCA9685_ADDR);
	haspcf8574 = scanaddress(PCF8574_ADDR);
	haspcf8575 = scanaddress(PCF8575_ADDR);
	hasMCP23017 = scanaddress(MCP23017_ADDR);

	if (hasBH1750 == true) {
		lightMeter = new BH1750();
		lightMeter->begin();
	}
	//lightMeter.begin();
	if (hasBME280 == true) {
		bme = new BME280I2C();
		bme->begin();
	}

}

bool I2Cexp::scanaddress(int address) {
	Wire.beginTransmission(address);
	byte result = Wire.endTransmission();
	return (result == TWI_SUCCESS);
}

bool I2Cexp::getMetrics(void) {
	bool retval = false;
	if (hasBME280 == true) {
		float temp(0), hum(0), pres(0), lux(0);
		uint8_t pressureUnit(4); // unit: B000 = Pa, B001 = hPa, B010 = Hg, B011 = atm, B100 = bar, B101 = torr, B110 = N/m^2, B111 = psi
		bme->read(pres, temp, hum, true, pressureUnit); // Parameters: (float& pressure, float& temp, float& humidity, bool hPa = true, bool celsius = false)
	    Serial.println("hum: "+String(hum));
		metrics.hum = hum;
		metrics.temp = temp;
		metrics.pres = pres;
		retval = true;
	}

	if (hasBH1750 == true) {
		float lux(0);
		lux = lightMeter->readLightLevel();
		metrics.luxvalue = lux;
		retval = true;
	}

	return retval;
}

bool I2Cexp::hasIOexpander(void){
	return (haspcf8575 || haspcf8574 );
}

bool I2Cexp::hasPWM(void){
	return hasPCA9685;
}

int I2Cexp::scanbus(void) {
	uint8_t error, address;

	Serial.println("Scanning...");

	nDevices = 0;
	for (address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == TWI_SUCCESS) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.print(address, HEX);
			Serial.println("  !");

			nDevices++;
		} else if (error == TWI_ERROR) {
			Serial.print("Unknown error at address 0x");
			if (address < 16)
				Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println("No I2C devices found\n");
	else
		Serial.println("done\n");

	return nDevices;
}
