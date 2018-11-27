#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include <Arduino.h>
//#include <NeoPixelBus.h>

class NeoPixel
{
  public:
    NeoPixel();
    void initPixel(void);

    void setPixelHsl(String pwmstr, String payload);
    void setFade(String pwmstr, String payload);
    void setFade(int index, String payload);
    void animate(void);
    


};

#endif /* NEOPIXEL_H_ */