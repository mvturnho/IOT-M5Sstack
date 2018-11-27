/*
 * target.h
 *
 *  Created on: 23 feb. 2017
 *      Author: michi_000
 */

#ifndef TARGET_H_
#define TARGET_H_
#include <M5Stack.h>
#define DEBUG

//Harware layout
#define LED_PIN 2
#define SETUP_PIN BUTTON_A_PIN
#define I2C_SDA_PIN 21
#define SW_PIN BUTTON_B_PIN
#define I2C_SCL_PIN 22
#define IOEXT_INT BUTTON_C_PIN
#define MOTION_PIN1 34
#define MOTION_PIN2 35 // what digital pin we're connected to
#define MOTION_PIN3 36

//EEPROM data locations
#define SSID_EPOS "sid"
#define PWD_EPOS "pwd"
#define OTAS_EPOS "ots"
#define OTAP_EPOS "otp"
#define OTAU_EPOS "otu"
#define MQTS_EPOS "mqs"
#define MQTP_EPOS "mqp"
#define MQTD_EPOS "mqd"
#define MQTL_EPOS "mql"
#define MQTC_EPOS "mqc"
#define MQTU_EPOS "mqu"
#define MQTW_EPOS "mqw"
#define NUMSTRIP "ns"
#define NUMOUTP "no"

#define EEPROM_MAX 300

#endif /* TARGET_H_ */
