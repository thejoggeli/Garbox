#pragma once

#include <cstdint>

#include "core/time/SoftwarePwm.h"
#include "core/time/SoftwareTimer.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "core/util/conditioner/AdcConditioner.h"

namespace Garbox {

class Adc;
class Gpio;

class Heatpad {
public:

    using StateChangedHandler = std::function<void(HeatpadState oldstate, HeatpadState newState)>;

    struct Config {
        Gpio& enableGpio;
        Adc& voltageSenseAdc; 
        Adc& currentSenseAdc;
        uint32_t adcFilterTicks = 10;
        float initialPwmDuty = 0.5f;
        uint32_t initialPwmPeriodMicros = 5'000'000;
    };

    Heatpad(const Config& config);

    void init();
    void tick();

    void setEnabled(bool enabled);
    void setDutyCycle(float duty);
    void setPeriodDurationMicros(float durationMicros);

    void setStateChangedHandler(StateChangedHandler handler);

    bool isEnabled() const;
    HeatpadState getState() const;
    float getCurrentDutyCycle() const;
    float getNextDutyCycle() const;
    uint32_t getCurrentPeriodDurationMicros() const;
    uint32_t getNextPeriodDurationMicros() const;
    float getMeasuredVoltage() const;
    float getMeasuredCurrent() const;

    uint32_t getPwmProgressMicros() const;
    float getPwmProgressRelative() const;

    // disallow copy and move 
    Heatpad(const Heatpad&) = delete;
    Heatpad& operator=(const Heatpad&) = delete;
    Heatpad(Heatpad&&) = delete;
    Heatpad& operator=(Heatpad&&) = delete;

private:

    void updateState();
    void handlePwmStateChanged(SoftwarePwm::State state);
    void setHeatEnabled(bool enabled);
    bool isHeating();

    bool mEnabled = false;
    HeatpadState mState = HeatpadState::Disabled;
    StateChangedHandler mStateChangedHandler = nullptr;
    
    Gpio& mGpioHeatpadEnable;

    // adc pins
    Adc& mVoltageSenseAdc;
    Adc& mCurrentSenseAdc;

    // adc filters
    AdcConditioner mVoltageSenseConditioner;
    AdcConditioner mCurrentSenseConditioner;

    SoftwarePwm mPwm;

    bool mHeatEnabled = false;

};

} // namespace