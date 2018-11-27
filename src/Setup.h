/*
 * Setup.h
 *
 *  Created on: 7 apr. 2017
 *      Author: michi_000
 */

#ifndef SETUP_H_
#define SETUP_H_

#include <Arduino.h>
#include <Preferences.h>

#include "target.h"

#define KEYLEN 20
#define VALLEN 30

class Setup
{
  public:
	Setup();
	void initSetup();
	void saveString(String value, String key);
	void saveInt(String value, String key);
	void cleareeprom(void);

	String getHTML();

	String getMqttclientid();
	String getMqttdevice();
	String getMqtthost();
	String getMqttlocation();
	String getMqttpassword();
	String getMqttport();
	String getMqttuser();
	int getNumleds();
	int getNumoutputs();
	String getOtahost();
	String getOtaport();
	String getOtaurl();
	String getPasswd();
	String getSsid();

  private:
	Preferences preferences;
	// String ssid = "IOT";
	// String passwd = "nopass";
	// String otahost = "192.168.3.100";
	// String otaport = "80";
	// String otaurl = "/esp/update/ota.php";
	// String mqtthost = "192.168.3.100";
	// String mqttport = "1883";
	// String mqttlocation = "unknown";
	// String mqttdevice = "default";
	// String mqttclientid = "";
	// String mqttuser = "";
	// String mqttpassword = "";

	int numleds = 5;
	int numoutputs = 0;
};

#endif /* SETUP_H_ */
