
#include "RgbLedStrip.h"

#include <Adafruit_NeoPixel.h>

#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/time/Time.h"

namespace Garbox {

RgbLedStrip::RgbLedStrip(Gpio& gpio, uint16_t count):
    // initialize members
    mGpio(gpio),
    mCount(count){

    // constructor body
    mRgbLeds = new Adafruit_NeoPixel(mCount, mGpio.getPinNumber(), NEO_GRB + NEO_KHZ800);
    AssertExit(mRgbLeds != nullptr, "RgbLedStrip", "allocation failed");
}

RgbLedStrip::~RgbLedStrip(){
    TriggerExit("RgbLedStrip", "heap using classes must not be deconstructed");
}

void RgbLedStrip::init(){
    AssertExit(!mInitialized, "RgbLedStrip", "already initialized");
    mRgbLeds->begin();
    mRgbLeds->show();
    mInitialized = true;
}

void RgbLedStrip::tick(){
    if(mNeedsUpdate){
        mRgbLeds->show();
        mNeedsUpdate = false;
    }
}

void RgbLedStrip::setAll(const Rgb888& color){
    setAll(color.r, color.g, color.b);
    mNeedsUpdate = true;
}

void RgbLedStrip::setAll(uint8_t r, uint8_t g, uint8_t b){
    const uint32_t color = mRgbLeds->Color(r, g, b);
    for(uint16_t i = 0; i < mCount; i++){
        mRgbLeds->fill(color, 0, mRgbLeds->numPixels());
    }
    mNeedsUpdate = true;
}

void RgbLedStrip::setPixelColor(uint16_t index, const Rgb888& color){
    setPixelColor(index, color.r, color.g, color.b);
    mNeedsUpdate = true;
}

void RgbLedStrip::setPixelColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b){
    if(index >= mCount){
        TriggerDebug("RgbLedStrip", "index out of bounds");
        return;
    }
    const uint32_t color = mRgbLeds->Color(r, g, b);
    mRgbLeds->setPixelColor(index, color);
    mNeedsUpdate = true;
}

void RgbLedStrip::fill(uint16_t start, uint16_t count, const Rgb888& color){
    fill(start, count, color.r, color.g, color.b);
    mNeedsUpdate = true;
}

void RgbLedStrip::fill(uint16_t start, uint16_t count, uint8_t r, uint8_t g, uint8_t b){
    if(start + count > mCount){
        TriggerDebug("RgbLedStrip", "fill index out of bounds");
        return;
    }
    const uint32_t color = mRgbLeds->Color(r, g, b);
    mRgbLeds->fill(color, start, count);
    mNeedsUpdate = true;
}

void RgbLedStrip::clear(){
    mRgbLeds->clear();
    mNeedsUpdate = true;
}

void RgbLedStrip::show(){
    if(mNeedsUpdate){
        mRgbLeds->show();
    }
    mNeedsUpdate = false;
}

void RgbLedStrip::setBrightness(uint8_t brightness){
    mRgbLeds->setBrightness(brightness);
    mNeedsUpdate = true;
}

void RgbLedStrip::setBrightnessFloat(float brightness){
    const float scaled = std::clamp(brightness * 255.0f + 0.5f, 0.0f, 255.0f);
    mRgbLeds->setBrightness(static_cast<uint8_t>(scaled));
    mNeedsUpdate = true;
}

uint16_t RgbLedStrip::getCount() const {
    return mCount;
}

uint8_t RgbLedStrip::getBrightness() const {
    return mRgbLeds->getBrightness();
}

float RgbLedStrip::getBrightnessFloat() const {
    return static_cast<float>(mRgbLeds->getBrightness()) / 255.0f;
}

bool RgbLedStrip::isInitialized() const {
    return mInitialized;
}

} // namespace
