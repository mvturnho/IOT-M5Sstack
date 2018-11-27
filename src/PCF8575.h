/*
 * PCF8575.h
 *
 *  Created on: 13 apr. 2017
 *      Author: michi_000
 */

#ifndef PCF8575_H_
#define PCF8575_H_

#include <Arduino.h>
#include <Wire.h>

// _________________________________________________________________ CONSTANTS
/// I2C/TWI success (transaction was successful).
static const uint8_t TWI_SUCCESS      = 0;
/// I2C/TWI device not present (address sent, NACK received).
static const uint8_t TWI_DEVICE_NACK  = 2;
/// I2C/TWI data not received (data sent, NACK received).
static const uint8_t TWI_DATA_NACK    = 3;
/// I2C/TWI other error.
static const uint8_t TWI_ERROR        = 4;


class PCF8575 {
public:
	// ............................................... public member functions
	PCF8575(uint8_t);
	PCF8575();
	uint8_t digitalRead();
	uint8_t digitalWrite(uint16_t);
	uint8_t getAddress();
	uint16_t getPorts();
	void enableBitwiseInversion();
	void disableBitwiseInversion();
	bool isInverted();

	// ........................................ static public member functions

	// ..................................................... public attributes

	// .............................................. static public attributes

private:
	// .............................................. private member functions

	// ....................................... static private member functions

	// .................................................... private attributes
	/// Device address as defined by pins A2, A1, A0.
	uint8_t address_;

	/// Storage object for I2cDiscreteIoExpander ports 1 (P17..P10), 0 (P07..P00).
	uint16_t ports_;

	/// Flag indicating whether bits are to be inverted before read/write (false=don't invert, true=invert).
	bool shouldInvert_;

	// ............................................. static private attributes
	/// Factory pre-set slave address.
	static const uint8_t BASE_ADDRESS_ = 0x20;
};

#endif /* PCF8575_H_ */
