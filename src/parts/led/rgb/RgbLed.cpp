#include "RgbLed.h"

#include <Adafruit_NeoPixel.h>

#include "assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/time/Time.h"
#include "util/color/types/HslColor.h"
#include "util/color/types/RgbFloat.h"
#include "util/color/types/Rgb888.h"

namespace Garbox {

RgbLed::RgbLed(Gpio& gpio):
    // initialize members
    mGpio(gpio){

    // constructor body
    mRgbLed = new Adafruit_NeoPixel(1, mGpio.getPinNumber(), NEO_GRB + NEO_KHZ800);
    AssertExit(mRgbLed != nullptr, "RgbLed", "allocation failed");
}

RgbLed::~RgbLed(){
    TriggerExit("RgbLed", "heap using classes must not be deconstructed");
}

void RgbLed::init(){
    AssertExit(!mInitialized, "RgbLed", "already initialized");
    mRgbLed->begin();
    mRgbLed->show();
    mInitialized = true;
}

void RgbLed::setColor(const HslColor& color){
    setColor(Rgb888::From(color));
}

void RgbLed::setColor(const RgbFloat& color){
    setColor(Rgb888::From(color));
}

void RgbLed::setColor(const Rgb888& color){
    setColor(color.r, color.g, color.b);
}

void RgbLed::setColor(uint8_t r, uint8_t g, uint8_t b){
    const uint32_t color = mRgbLed->Color(r, g, b);
    mRgbLed->setPixelColor(0, color);
    mRgbLed->show();
}

bool RgbLed::isInitialized() const {
    return mInitialized;
}

} // namespace
