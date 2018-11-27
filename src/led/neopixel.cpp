#include "neopixel.h"
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

const uint16_t PixelCount = 15; // make sure to set this to the number of pixels in your strip
const uint16_t PixelPin = 17;
const uint8_t AnimationChannels = PixelCount;

#define colorSaturation 128

const uint16_t PixelFadeDuration = 300; // third of a second
// one second divide by the number of pixels = loop once a second
const uint16_t NextPixelMoveDuration = 1000 / PixelCount; // how fast we move through the pixels

void LoopAnimUpdate(const AnimationParam& param);

RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

HslColor hslRed(red);
HslColor hslGreen(green);
HslColor hslBlue(blue);
HslColor hslWhite(white);
HslColor hslBlack(black);

NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
NeoPixelAnimator animations(AnimationChannels);

struct MyAnimationState
{
    RgbColor StartingColor;
    RgbColor EndingColor;
    uint16_t IndexPixel;
};

MyAnimationState animationState[AnimationChannels];
uint16_t frontPixel = 0;  // the front of the loop
RgbColor frontColor;  // the color at the front of the loop

NeoPixel::NeoPixel()
{
}

void NeoPixel::initPixel()
{
    strip.Begin();
    strip.SetPixelColor(0, red);
    strip.Show();
}

void NeoPixel::setPixelHsl(String pwmstr, String payload)
{
    int index = pwmstr.toInt();
    int h_start = payload.indexOf("(");
    int s_start = payload.indexOf(",", h_start);
    int l_start = payload.indexOf(",", s_start + 1);

    String sh = payload.substring(h_start + 1, s_start);
    String ss = payload.substring(s_start + 1, l_start);
    String sl = payload.substring(l_start + 1, payload.length() - 1);

    float h = sh.toInt();
    float s = ss.toInt();
    float l = sl.toInt();

    HslColor result = HslColor(h / 100.0, s / 100.0, l / 100.0);
    strip.SetPixelColor(index, result);
    strip.Show();
}

void NeoPixel::setFade(String pwmstr, String payload)
{
    if (pwmstr.equals("*"))
    { //all led strips on device
        for (int i = 0; i < 15; i++)
            setFade(i, payload);
    }
    else
    {
        int stripindex = pwmstr.toInt();
        setFade(stripindex, payload);
    }
}

void NeoPixel::setFade(int index, String payload)
{
    int start = payload.indexOf('(');
    int time = 0;
    int light = 0;
    String param = payload.substring(start + 1, payload.length() - 1);
    start = param.indexOf(',');
    int state = param.substring(0, start).toInt();

    //	String time = param.substring(start + 1, param.length());
    //	Serial.println("state=" + state + " fadetime=" + time);
    animations.StartAnimation(0, NextPixelMoveDuration, LoopAnimUpdate);
}

void FadeOutAnimUpdate(const AnimationParam& param)
{
    // this gets called for each animation on every time step
    // progress will start at 0.0 and end at 1.0
    // we use the blend function on the RgbColor to mix
    // color based on the progress given to us in the animation
    RgbColor updatedColor = RgbColor::LinearBlend(
        animationState[param.index].StartingColor,
        animationState[param.index].EndingColor,
        param.progress);
    // apply the color to the strip
    strip.SetPixelColor(animationState[param.index].IndexPixel, 
        colorGamma.Correct(updatedColor));
}

void LoopAnimUpdate(const AnimationParam& param)
{
    // wait for this animation to complete,
    // we are using it as a timer of sorts
    if (param.state == AnimationState_Completed)
    {
        // done, time to restart this position tracking animation/timer
        animations.RestartAnimation(param.index);

        // pick the next pixel inline to start animating
        // 
        frontPixel = (frontPixel + 1) % PixelCount; // increment and wrap
        if (frontPixel == 0)
        {
            // we looped, lets pick a new front color
            frontColor = HslColor(random(360) / 360.0f, 1.0f, 0.25f);
        }

        uint16_t indexAnim;
        // do we have an animation available to use to animate the next front pixel?
        // if you see skipping, then either you are going to fast or need to increase
        // the number of animation channels
        if (animations.NextAvailableAnimation(&indexAnim, 1))
        {
            animationState[indexAnim].StartingColor = frontColor;
            animationState[indexAnim].EndingColor = RgbColor(0, 0, 0);
            animationState[indexAnim].IndexPixel = frontPixel;

            animations.StartAnimation(indexAnim, PixelFadeDuration, FadeOutAnimUpdate);
        }
    }
}
void NeoPixel::animate(void)
{

    // the normal loop just needs these two to run the active animations
    animations.UpdateAnimations();
    strip.Show();
}