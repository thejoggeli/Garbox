#include "RgbLed.h"

#include <Adafruit_NeoPixel.h>

#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/time/Time.h"
#include "core/util/color/types/HslColor.h"
#include "core/util/color/types/RgbFloat.h"
#include "core/util/color/types/Rgb888.h"

namespace Garbox {

RgbLed::RgbLed(Gpio& gpio):
    // initialize members
    mGpio(gpio){
    // constructor body
}

RgbLed::~RgbLed(){
    TriggerExit("RgbLed", "heap using classes must not be deconstructed");
}

void RgbLed::init(){
    AssertExit(!mInitialized, "RgbLed", "already initialized");
    mRgbLed = new Adafruit_NeoPixel(1, mGpio.getPinNumber(), NEO_GRB + NEO_KHZ800);
    AssertExit(mRgbLed != nullptr, "RgbLed", "allocation failed");
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

    // check if needs update
    if(!mNeedsUpdate){

        // check if color changed
        const bool colorChanged =
            mCurrentRed   != r || 
            mCurrentGreen != g ||
            mCurrentBlue  != b;

        if(!colorChanged){
            return;
        }        
    }
    else {
        mNeedsUpdate = false;
    }
    
    // write color to rgb led
    mRgbLed->setPixelColor(0, r, g, b);
    mRgbLed->show();
}

bool RgbLed::isInitialized() const {
    return mInitialized;
}

void RgbLed::setNeedsUpdate(){
    mNeedsUpdate = true;
}

} // namespace
