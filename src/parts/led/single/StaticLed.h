#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

namespace Garbox {

class Gpio;

class StaticLed {
public:

    StaticLed(Gpio& gpio);

    virtual void init();
    virtual void toggle();
    virtual void setEnabled(bool enabled);

    bool isEnabled() const;

    // Disallow copy and move 
    StaticLed(const StaticLed&) = delete;
    StaticLed& operator=(const StaticLed&) = delete;
    StaticLed(StaticLed&&) = delete;
    StaticLed& operator=(StaticLed&&) = delete;

private:
    Gpio& mGpio;
    SemaphoreHandle_t mMutex = nullptr;
};

} // namespace
