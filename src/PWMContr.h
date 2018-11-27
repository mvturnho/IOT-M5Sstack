/*
 * PWMContr.h
 *
 *  Created on: 6 apr. 2017
 *      Author: michi_000
 */

#ifndef PWMCONTR_H_
#define PWMCONTR_H_

#include <Adafruit_PWMServoDriver.h>

#define PWMCHAN 16
#define MAXPWM 4095
#define MAXBRIGHT 100
#define MAXLEDSSTRIPS 5
#define H2R_MAX_RGB_val 4095.0
#define NUMCOLORS  1024

#define H 0
#define S 1
#define L 2
#define Ls 3

#define R 0
#define G 1
#define B 2

class PWMContr {
private:
	struct controldata {
		int pwmindex = 0;
		uint16_t pwm[3];
		uint16_t hsl[4];
		int anim = 0;
		int pulse = 0;
		int fade = 0;
		int state = 1;
		int colorcounter = 0;
		int numColors = NUMCOLORS;
		unsigned long animLastMillis = 0;
		unsigned long fadeLastMillis = 0;
	};

	Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
	int numleds = MAXLEDSSTRIPS;
	controldata control[MAXLEDSSTRIPS];

	void setHSL(int index);
	void setHSL(int index, uint16_t h, uint16_t s, uint16_t l);
	void setPWM(int index, uint16_t r, uint16_t g, uint16_t b);
	void setPWM(int index, uint16_t *colors);
	void HSBtoRGB(int hue, int sat, int bright, uint16_t *colors);
	void HSBtoRGB(int index);

	void writePWM(int index);
	void writePWM(int index, uint16_t *colors);
	void writePWM(int index, uint16_t r, uint16_t g, uint16_t b);

public:
	unsigned long animationtime_ms = 10;

	PWMContr();
	void initPWM(int activeleds);
	void switchLedStrip(String pwmstr, String payload);
	void switchLedStrip(int index, String payload);
	void pwmLedStrip(String pwmstr, String payload);
	void rgbLedStrip(String pwmstr, String payload);
	void hslLedStrip(String pwmstr, String payload);
	void setAnimate(String pwmstr, String payload);
	void setAnimate(int index, String payload);
	void setPulse(String pwmstr, String payload);
	void setPulse(int index, String payload);
	void setFade(String pwmstr, String payload);
	void setFade(int index, String payload);
	void setFade(int index, int state, int time);
	void dumpPwms(uint16_t *values);
	void animate(void);
};

#endif /* PWMCONTR_H_ */
