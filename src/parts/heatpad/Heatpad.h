#pragma once

#include <cstdint>

#include "core/time/SoftwarePwm.h"
#include "core/time/SoftwareTimer.h"
#include "util/conditioner/AdcConditioner.h"

namespace Garbox {

class Adc;
class Gpio;

class Heatpad {
public:

    Heatpad(Gpio& enableGpio, Adc& voltageSenseAdc, Adc& currentSenseAdc);

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
    float getMeasuredVoltage() const;
    float getMeasuredCurrent() const;

    // Disallow copy and move 
    Heatpad(const Heatpad&) = delete;
    Heatpad& operator=(const Heatpad&) = delete;
    Heatpad(Heatpad&&) = delete;
    Heatpad& operator=(Heatpad&&) = delete;

private:

    void handlePwmStateChanged(SoftwarePwm::State state);

    void setHeatEnabled(bool enabled);
    bool isHeatEnabled();
    
    Gpio& mGpioHeatpadEnable;

    // adc pins
    Adc& mVoltageSenseAdc;
    Adc& mCurrentSenseAdc;

    // adc filters
    AdcConditioner mVoltageSenseConditioner;
    AdcConditioner mCurrentSenseConditioner;

    SoftwarePwm mPwm;
    SoftwareTimer mLogTimer;

    bool mHeatEnabled = false;

};

} // namespace