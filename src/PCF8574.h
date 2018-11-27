/*
 * PCF8574.h
 *
 *  Created on: 10 apr. 2017
 *      Author: michi_000
 */

#ifndef PCF8574_H_
#define PCF8574_H_


#include "Arduino.h"


#define PCF8574_LIB_VERSION "0.1.02"

class PCF8574
{
  public:
  PCF8574(int address);

  uint8_t digitalRead();
  uint8_t digitalRead(uint8_t pin);
  uint8_t value();

  int digitalWrite(uint8_t value);
  void digitalWrite(uint8_t pin, uint8_t value);

  void toggle(uint8_t pin);
  void shiftRight(uint8_t n=1);
  void shiftLeft(uint8_t n=1);

  int lastError();

  private:
  int _address;
  uint8_t _data;
  int _error;
};

#endif /* PCF8574_H_ */
