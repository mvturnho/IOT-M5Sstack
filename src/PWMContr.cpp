/*
 * PWMContr.cpp
 *
 *  Created on: 6 apr. 2017
 *      Author: michi_000
 */
#include <Arduino.h>
#include "PWMContr.h"

PWMContr::PWMContr() {
}

void PWMContr::initPWM(int activeleds) {
	pwm.begin();
	pwm.setPWMFreq(1600);  // This is the maximum PWM frequency
	numleds = activeleds;

	for (int i = 0; i < numleds; i++) {
		control[i].pwmindex = i;
		control[i].pwm[R] = 0;
		control[i].pwm[G] = 0;
		control[i].pwm[B] = 0;
		control[i].hsl[H] = 0;
		control[i].hsl[S] = 0;
		control[i].hsl[L] = 0;
		control[i].hsl[Ls] = 0;
		control[i].fade = 0;
		control[i].anim = 0;
		control[i].state = 0;
		control[i].colorcounter = 0;

		writePWM(i);
	}
}

void PWMContr::switchLedStrip(String pwmstr, String payload) {
	if (pwmstr.equals("*")) {
		for (int i = 0; i < numleds; i++) {
			switchLedStrip(i, payload);
		}
	} else {
		int pwmindex = pwmstr.toInt();
		switchLedStrip(pwmindex, payload);
	}
}

void PWMContr::switchLedStrip(int index, String payload) {
	control[index].fade = 0;
	if (payload.equals("off")) {
		Serial.println(" off");
		control[index].state = 0;
	} else {
		Serial.println(" on");
		control[index].state = 1;
	}
	writePWM(index);
}

void PWMContr::pwmLedStrip(String pwmstr, String payload) {

	int r_start = payload.indexOf("(");
	int g_start = payload.indexOf(",", r_start);
	int b_start = payload.indexOf(",", g_start + 1);

	int r = payload.substring(r_start + 1, g_start).toInt();
	int g = payload.substring(g_start + 1, b_start).toInt();
	int b = payload.substring(b_start + 1, payload.length() - 1).toInt();

	if (pwmstr.equals("*")) {
		for (int i = 0; i < numleds; i++) {
			setPWM(i, r, g, b);
		}
	} else {
		int pwmindex = pwmstr.toInt();
		setPWM(pwmindex, r, g, b);
	}

}

void PWMContr::rgbLedStrip(String pwmstr, String payload) {
	int pwmindex = pwmstr.toInt();
	uint8_t pwmnum = pwmindex * 3;

	int r_start = payload.indexOf("(");
	int g_start = payload.indexOf(",", r_start);
	int b_start = payload.indexOf(",", g_start + 1);

	int r = payload.substring(r_start + 1, g_start).toInt() * 16;
	int g = payload.substring(g_start + 1, b_start).toInt() * 16;
	int b = payload.substring(b_start + 1, payload.length() - 1).toInt() * 16;

//Serial.println("R:" + String(r) + " G:" + String(g) + " B:" + String(b));

	setPWM(pwmindex, r, g, b);
}

void PWMContr::hslLedStrip(String pwmstr, String payload) {
	int pwmindex = pwmstr.toInt();
	uint8_t pwmnum = pwmindex * 3;

	int h_start = payload.indexOf("(");
	int s_start = payload.indexOf(",", h_start);
	int l_start = payload.indexOf(",", s_start + 1);

	String sh = payload.substring(h_start + 1, s_start);
	String ss = payload.substring(s_start + 1, l_start);
	String sl = payload.substring(l_start + 1, payload.length() - 1);
	int h = 0;
	int s = 0;
	int l = 0;

	if (control[pwmindex].anim > 0) {
		sh = "?";
		ss = "?";
	}

	if (sh.equals("?"))
		h = control[pwmindex].hsl[H];
	else
		h = sh.toInt();
	if (ss.equals("?"))
		s = control[pwmindex].hsl[S];
	else
		s = ss.toInt();
	if (sl.equals("?"))
		l = control[pwmindex].hsl[L];
	else
		l = sl.toInt();

//Serial.println("HSL:" + sh + "," + ss + "," + sl);

	if (pwmstr.equals("*")) {
		for (int i = 0; i < numleds; i++) {
			setHSL(i, h, s, l);
		}
	} else {
		int pwmindex = pwmstr.toInt();
		setHSL(pwmindex, h, s, l);
	}
}

void PWMContr::setPWM(int index, uint16_t *colors) {
	setPWM(index, colors[0], colors[1], colors[2]);
}

void PWMContr::setPWM(int index, uint16_t r, uint16_t g, uint16_t b) {
    Serial.println("R:" + String(r) + " G:" + String(g) + " B:" + String(b));
	r = constrain(r, 0, MAXPWM);
	g = constrain(g, 0, MAXPWM);
	b = constrain(b, 0, MAXPWM);

	control[index].pwm[R] = r;
	control[index].pwm[G] = g;
	control[index].pwm[B] = b;

	writePWM(index);
}

void PWMContr::setHSL(int index) {
	HSBtoRGB(index);
	writePWM(index);
}

void PWMContr::setHSL(int index, uint16_t h, uint16_t s, uint16_t l) {
	control[index].hsl[H] = h;
	control[index].hsl[S] = s;
	control[index].hsl[L] = l;
	control[index].hsl[Ls] = l;

	HSBtoRGB(h, s, l, control[index].pwm);
	//writePWM(index);
}

void PWMContr::writePWM(int index) {
	int pwmnum = index * 3;
	if ((control[index].state == 0) && (control[index].fade == 0)) {
		writePWM(index, 0, 0, 0);
	} else {
		pwm.setPWM(pwmnum, 0, control[index].pwm[R]);
		pwm.setPWM(pwmnum + 1, 0, control[index].pwm[G]);
		pwm.setPWM(pwmnum + 2, 0, control[index].pwm[B]);
	}
}

void PWMContr::writePWM(int index, uint16_t *colors) {
	int pwmnum = index * 3;
	pwm.setPWM(pwmnum, 0, colors[R]);
	pwm.setPWM(pwmnum + 1, 0, colors[G]);
	pwm.setPWM(pwmnum + 2, 0, colors[B]);
}

void PWMContr::writePWM(int index, uint16_t r, uint16_t g, uint16_t b) {
	r = constrain(r, 0, MAXPWM);
	g = constrain(g, 0, MAXPWM);
	b = constrain(b, 0, MAXPWM);

	int pwmnum = index * 3;
	pwm.setPWM(pwmnum, R, r);
	pwm.setPWM(pwmnum + G, 0, g);
	pwm.setPWM(pwmnum + B, 0, b);
}

void PWMContr::dumpPwms(uint16_t *values) {
	for (int i = 0; i < PWMCHAN; i++) {
		Serial.println("channel: " + String(i) + " - " + values[i]);
	}
}

void PWMContr::setAnimate(String pwmstr, String payload) {
	if (pwmstr.equals("*")) {
		for (int i = 0; i < numleds; i++)
			setAnimate(i, payload);

	} else {
		int stripindex = pwmstr.toInt();
		setAnimate(stripindex, payload);
	}
}

void PWMContr::setAnimate(int index, String payload) {
	int state = 1;
	if (payload.endsWith("(0)")) {
		state = 0;
	}
	if (state != control[index].state) {
		if (state == 0) {
			control[index].anim = 0;
			control[index].state = 0;
			control[index].hsl[Ls] = control[index].hsl[L];
			writePWM(index); //immediate off
		} else {
			int start = payload.indexOf('(');
			String tm = payload.substring(start + 1, payload.length() - 1);
			Serial.println("animtime = " + tm);
			control[index].anim = tm.toInt();
			control[index].state = 1;
			control[index].hsl[L] = control[index].hsl[Ls]; //restore bright
		}
	}
}

void PWMContr::setPulse(String pwmstr, String payload) {
	if (pwmstr.equals("*")) {
		for (int i = 0; i < numleds; i++)
			setPulse(i, payload);

	} else {
		int stripindex = pwmstr.toInt();
		setPulse(stripindex, payload);
	}
}

void PWMContr::setPulse(int index, String payload) {
	int time = control[index].pulse;
	int light = 0;
	int start = payload.indexOf('(');
	String param = payload.substring(start + 1, payload.length() - 1);
	start = param.indexOf(',');
	int state = param.substring(0, start).toInt();
	if (state == 1) {
		int lstart = param.indexOf(',', start + 1);
		time = param.substring(start + 1, lstart).toInt();

		String lights = param.substring(lstart + 1, param.length());
		//Serial.println("lights=" + lights + " start=" + start + " lstart=" + lstart);
		if (!lights.equals("?"))
			control[index].hsl[Ls] = lights.toInt();
		control[index].pulse = time;
	} else {
		control[index].pulse = 0;
	}

	Serial.println("state=" + String(state) + " pulsetime=" + String(time) + " Light=" + String(control[index].hsl[L]));

	setFade(index, state, time);
}

void PWMContr::setFade(String pwmstr, String payload) {
	if (pwmstr.equals("*")) { //all led strips on device
		for (int i = 0; i < numleds; i++)
			setFade(i, payload);
	} else {
		int stripindex = pwmstr.toInt();
		setFade(stripindex, payload);
	}
}

void PWMContr::setFade(int index, String payload) {
	int start = payload.indexOf('(');
	int time = 0;
	int light = 0;
	String param = payload.substring(start + 1, payload.length() - 1);
	start = param.indexOf(',');
	int state = param.substring(0, start).toInt();
//	String time = param.substring(start + 1, param.length());
//	Serial.println("state=" + state + " fadetime=" + time);

	if (state == 1) {
		int lstart = param.indexOf(',', start + 1);
		time = param.substring(start + 1, lstart).toInt();
		String lights = param.substring(lstart + 1, param.length());
		if (!lights.equals("?")) //use current Light or new value
			control[index].hsl[Ls] = lights.toInt();
		//Serial.println("lights=" + lights + " start=" + start + " lstart=" + lstart);
	} else {
		int lstart = param.indexOf(',', start + 1);
		time = param.substring(start + 1, param.length()).toInt();
	}

	if (state != control[index].state) {
		setFade(index, state, time);
	}
}

void PWMContr::setFade(int index, int state, int time) {
	control[index].state = state;
	control[index].fade = time;

	if (control[index].state == 0)
		control[index].hsl[Ls] = control[index].hsl[L];  //save the bright for fade in or switch on
	else if (control[index].hsl[Ls] == control[index].hsl[L])
		control[index].hsl[L] = 0;
}

void PWMContr::animate(void) {
	for (int i = 0; i < numleds; i++) { //LOOP all led strips
		if (control[i].state == 1) {  //STATE is on
			if (control[i].anim > 0) {  //ANIMATION HANDLE
				if (millis() - control[i].animLastMillis > control[i].anim) {
					control[i].animLastMillis = millis();
					//Serial.println("anim: " + String(i));
					float colorNumber =
							control[i].colorcounter > control[i].numColors ? control[i].colorcounter - control[i].numColors : control[i].colorcounter;
					int h = (colorNumber / float(control[i].numColors)) * 360;
					int s = 100;
					int l = control[i].hsl[2];
					setHSL(i, h, s, l);
					control[i].colorcounter = (control[i].colorcounter + 1) % (control[i].numColors * 2);
				}
			}
			if ((control[i].fade > 0) && (control[i].hsl[L] < control[i].hsl[Ls])) {  //FADE IN HANDLE
				if (millis() - control[i].fadeLastMillis > control[i].fade) {
					control[i].fadeLastMillis = millis();
					control[i].hsl[L]++;
					setHSL(i);
					//Serial.print(">");
				}
			} else {
				if (control[i].pulse > 0) {  //PULSE FADE OUT INITIALIZE
					setFade(i, 0, control[i].pulse);
				}
			}
		} else {  //STATE is OFF
			if ((control[i].fade > 0) && (control[i].hsl[L] > 0)) {  //FADE OUT HANDLE
				if (millis() - control[i].fadeLastMillis > control[i].fade) {
					control[i].fadeLastMillis = millis();
					control[i].hsl[L]--;
					setHSL(i);
					//Serial.print("<");
				}
			} else {
				//PULSE FADE IN INITIALIZE
				if (control[i].pulse > 0) {
					setFade(i, 1, control[i].pulse);
				}
			}
		}
		yield(); //service ESP8266 WIFI
	}
}

void PWMContr::HSBtoRGB(int index) {
	int h = control[index].hsl[H];
	int s = control[index].hsl[S];
	int l = control[index].hsl[L];

	HSBtoRGB(h, s, l, control[index].pwm);

}

void PWMContr::HSBtoRGB(int hue, int sat, int bright, uint16_t *colors) {

// constrain all input variables to expected range
	hue = constrain(hue, 0, 360);
	sat = constrain(sat, 0, 100);
	bright = constrain(bright, 0, MAXBRIGHT);

// define maximum value for RGB array elements
	float max_rgb_val = H2R_MAX_RGB_val;

// convert saturation and brightness value to decimals and init r, g, b variables
	float sat_f = float(sat) / 100.0;
	float bright_f = float(bright) / float(MAXBRIGHT);
	int r, g, b;

// If brightness is 0 then color is black (achromatic)
// therefore, R, G and B values will all equal to 0
	if (bright <= 0) {
		colors[R] = 0;
		colors[G] = 0;
		colors[B] = 0;
	} else {

		// If saturation is 0 then color is gray (achromatic)
		// therefore, R, G and B values will all equal the current brightness
		if (sat <= 0) {
			colors[R] = bright_f * max_rgb_val;
			colors[G] = bright_f * max_rgb_val;
			colors[B] = bright_f * max_rgb_val;
		} else {
			// if saturation and brightness are greater than 0 then calculate
			// R, G and B values based on the current hue and brightness

			if (hue >= 0 && hue < 120) {
				float hue_primary = 1.0 - (float(hue) / 120.0);
				float hue_secondary = float(hue) / 120.0;
				float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
				float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
				float sat_tertiary = 1.0 - sat_f;
				r = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
				g = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
				b = (bright_f * max_rgb_val) * sat_tertiary;
			}

			else if (hue >= 120 && hue < 240) {
				float hue_primary = 1.0 - ((float(hue) - 120.0) / 120.0);
				float hue_secondary = (float(hue) - 120.0) / 120.0;
				float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
				float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
				float sat_tertiary = 1.0 - sat_f;
				r = (bright_f * max_rgb_val) * sat_tertiary;
				g = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
				b = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
			}

			else if (hue >= 240 && hue <= 360) {
				float hue_primary = 1.0 - ((float(hue) - 240.0) / 120.0);
				float hue_secondary = (float(hue) - 240.0) / 120.0;
				float sat_primary = (1.0 - hue_primary) * (1.0 - sat_f);
				float sat_secondary = (1.0 - hue_secondary) * (1.0 - sat_f);
				float sat_tertiary = 1.0 - sat_f;
				r = (bright_f * max_rgb_val) * (hue_secondary + sat_secondary);
				g = (bright_f * max_rgb_val) * sat_tertiary;
				b = (bright_f * max_rgb_val) * (hue_primary + sat_primary);
			}

			colors[R] = r;
			colors[G] = g;
			colors[B] = b;
		}

	}
}
