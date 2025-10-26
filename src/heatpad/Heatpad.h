#pragma once

#include <cstdint>

#include "core/Gpio.h" 
#include "core/SoftwarePwm.h"

namespace Garbox {

class Heatpad {
public:

    Heatpad();

    void init();
    void start();
    void tick();
    void reset();

    void setDutyCycle(float duty);
    float getCurrentDutyCycle();
    float getNextDutyCycle();

private:

    void handlePwmStateChanged(SoftwarePwm::State state);

    void setHeatEnabled(bool enabled);
    bool isHeatEnabled();

    static constexpr uint32_t PwmPeriodMillis = 5000; // 5 seconds

    Gpio mGpioHeatpadEnable;
    SoftwarePwm mPwm;

    bool mHeatEnabled = false;

};

} // namespace