/*
 * Setup.cpp
 *
 *  Created on: 7 apr. 2017
 *      Author: michi_000
 */

#include "Setup.h"
#include <WiFi.h>

Setup::Setup()
{
}

void Setup::initSetup()
{
	preferences.begin("iot-config");
}

void Setup::cleareeprom(void)
{
	preferences.clear();
}

void Setup::saveString(String value, String key)
{
	char c_key[KEYLEN];
	char c_value[VALLEN];
	key.toCharArray(c_key, KEYLEN);
	value.toCharArray(c_value, VALLEN);
	preferences.putString(c_key, c_value);
	Serial.println(key + ":" + value);
}

void Setup::saveInt(String value, String key)
{
	char c_key[KEYLEN];
	key.toCharArray(c_key, KEYLEN);
	preferences.putInt(c_key, value.toInt());
	Serial.println(key + ":" + value);
}

String Setup::getHTML()
{
	String content = "";
	content += "</td><td><label>password: </label></td><td><input type='text' name='pass' value='" + preferences.getString(PWD_EPOS) + "' length=64></td></tr>";
	content += "<tr><td><label>OTA host: </label></td><td><input name='otaserver' value='" + preferences.getString(OTAS_EPOS) + "' length=32></td><td><label>OTA port: </label></td><td><input name='otaport' value='" + preferences.getString(OTAP_EPOS) + "' length=8></td></tr>";
	content += "<tr><td><label>OTA url: </label></td><td><input name='otaurl' value='" + preferences.getString(OTAU_EPOS) + "' length=32></td></tr>";
	content += "<tr><td><label>MQTT: </label></td><td><input name='mqttserver' value='" + preferences.getString(MQTS_EPOS) + "' length=32></td><td><label>port: </label></td><td><input name='mqttport' value='" + preferences.getString(MQTP_EPOS) + "' length=8></td></tr>";
	content += "<tr><td><label>MQTT dev: </label></td><td><input name='mqttdevice' value='" + preferences.getString(MQTD_EPOS) + "' length=16></td><td><label>MQTT location: </label></td><td><input name='mqttlocation' value='" + preferences.getString(MQTL_EPOS) + "' length=16></td></tr>";
	content += "<tr><td><label>MQTT clientid: </label></td><td><input name='mqttclid' value='" + preferences.getString(MQTC_EPOS) + "' length=32></td></tr>";
	content += "<tr><td><label>MQTT user: </label></td><td><input name='mqttuser' value='" + preferences.getString(MQTU_EPOS) + "' length=16></td><td><label>MQTT password: </label></td><td><input name='mqttpwd' value='" + preferences.getString(MQTW_EPOS) + "' length=16></td></tr>";
	content += "<tr><td><label>#Led Strips: </label></td><td><input name='numleds' value='" + String(preferences.getInt(NUMSTRIP)) + "' length=8></td></tr>";
	content += "<tr><td><label>#PCF8575: </label></td><td><input name='numoutputs' value='" + String(preferences.getInt(NUMOUTP)) + "' length=8></td></tr>";
	return content;
}

String Setup::getMqttclientid()
{
	return preferences.getString(MQTC_EPOS);
}

String Setup::getMqttdevice()
{
	return preferences.getString(MQTD_EPOS); // mqttdevice;
}

String Setup::getMqtthost()
{
	return preferences.getString(MQTS_EPOS); //  mqtthost;
}

String Setup::getMqttlocation()
{
	return preferences.getString(MQTL_EPOS); //  mqttlocation;
}

String Setup::getMqttpassword()
{
	return preferences.getString(MQTW_EPOS); //  mqttpassword;
}

String Setup::getMqttport()
{
	return preferences.getString(MQTP_EPOS); //  mqttport;
}

String Setup::getMqttuser()
{
	return preferences.getString(MQTU_EPOS); //  mqttuser;
}

int Setup::getNumleds()
{
	return preferences.getInt(NUMSTRIP); //numleds;
}

int Setup::getNumoutputs()
{
	return preferences.getInt(NUMOUTP); //numoutputs;
}

String Setup::getOtahost()
{
	return preferences.getString(OTAS_EPOS); //  otahost;
}

String Setup::getOtaport()
{
	return preferences.getString(OTAP_EPOS); //  otaport;
}

String Setup::getOtaurl()
{
	return preferences.getString(OTAU_EPOS); //  otaurl;
}

String Setup::getPasswd()
{
	return preferences.getString(PWD_EPOS); //  passwd;
}

String Setup::getSsid()
{
	return preferences.getString(SSID_EPOS); //  ssid;
}
