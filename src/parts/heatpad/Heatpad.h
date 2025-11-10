#pragma once

#include <cstdint>

#include "core/time/SoftwarePwm.h"
#include "core/time/SoftwareTimer.h"
#include "util/filter/SensorFilter.h"

namespace Garbox {

class Adc;
class Gpio;

class Heatpad {
public:

    Heatpad();

    void init();
    void start();
    void tick();
    void reset();

    void setDutyCycle(float duty);
    void setPeriodDurationMicros(float durationMicros);

    float getCurrentDutyCycle() const;
    float getNextDutyCycle() const;
    uint32_t getCurrentPeriodDurationMicros() const;
    uint32_t getNextPeriodDurationMicros() const;

private:

    void handlePwmStateChanged(SoftwarePwm::State state);

    void setHeatEnabled(bool enabled);
    bool isHeatEnabled();
    
    Gpio& mGpioHeatpadEnable;
    Adc& mVoltageSenseAdc;
    Adc& mCurrentSenseAdc;

    SoftwarePwm mPwm;
    SoftwareTimer mLogTimer;

    // adc filters
    SensorFilter mVoltageSenseFilter;
    SensorFilter mCurrentSenseFilter;

    bool mHeatEnabled = false;

};

} // namespace