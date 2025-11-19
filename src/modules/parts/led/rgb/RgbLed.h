#pragma once

#include <cstdint>

class Adafruit_NeoPixel;

namespace Garbox {

class Gpio;
class HslColor;
class RgbFloat;
class Rgb888;

class RgbLed {
public:
    RgbLed(Gpio& gpio);
    ~RgbLed();

    void init();

    void setColor(const HslColor& color);
    void setColor(const RgbFloat& color);
    void setColor(const Rgb888& color);
    void setColor(uint8_t r, uint8_t g, uint8_t b);

    bool isInitialized() const;

    // disallow copy and move 
    RgbLed(const RgbLed&) = delete;
    RgbLed& operator=(const RgbLed&) = delete;
    RgbLed(RgbLed&&) = delete;
    RgbLed& operator=(RgbLed&&) = delete;

private:
    Adafruit_NeoPixel* mRgbLed = nullptr;
    Gpio& mGpio;
    bool mInitialized = false;
    bool mNeedsUpdate = false;
};

} // namespace
