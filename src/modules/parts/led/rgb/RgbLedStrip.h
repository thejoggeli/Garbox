#pragma once

#include <cstdint>
#include "core/util/color/types/Rgb888.h"

class Adafruit_NeoPixel;

namespace Garbox {

class Gpio;

class RgbLedStrip {
public:
    RgbLedStrip(Gpio& gpio, uint16_t count);
    ~RgbLedStrip();

    void init();
    void tick();

    void setAll(const Rgb888& color);
    void setAll(uint8_t r, uint8_t g, uint8_t b);

    void setPixelColor(uint16_t index, const Rgb888& color);
    void setPixelColor(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

    void fill(uint16_t start, uint16_t count, const Rgb888& color);
    void fill(uint16_t start, uint16_t count, uint8_t r, uint8_t g, uint8_t b);

    void clear();
    void show();
    void setBrightness(uint8_t brightness);
    void setBrightnessFloat(float brightness);

    // added optional useful methods
    uint16_t getCount() const;
    uint8_t getBrightness() const;
    float getBrightnessFloat() const;
    bool isInitialized() const;

    // disallow copy and move 
    RgbLedStrip(const RgbLedStrip&) = delete;
    RgbLedStrip& operator=(const RgbLedStrip&) = delete;
    RgbLedStrip(RgbLedStrip&&) = delete;
    RgbLedStrip& operator=(RgbLedStrip&&) = delete;

private:
    Adafruit_NeoPixel* mRgbLeds = nullptr;
    Gpio& mGpio;
    uint16_t mCount;
    bool mInitialized = false;
    bool mNeedsUpdate = false;
};

} // namespace
