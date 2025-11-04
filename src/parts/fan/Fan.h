#pragma once

#include <cstdint>
#include "FanStateMonitor.h"
#include "core/sensor/FrequencySensor.h"
#include "util/filter/ExponentialFilter.h"

namespace Garbox {

class Gpio;
class LedcChannel;

class Fan {
public:

    enum class State : uint8_t {
        Off = 0,
        On,
        Stalled
    };

    using StateChangedCallback = std::function<void(State state)>;

    Fan();

    void init();
    void start();
    void tick();

    void setStateChangedCallback(StateChangedCallback callback);
    void setEnabled(bool enabled);
    void setSpeed(float speed); // range [0.0, 1.0]

    bool isEnabled();
    float getSpeed();
    float getMeasuredRpm(bool filtered = true);

private:

    void handleFanStateMonitorCallback(FanStateMonitor::State state);

    State mState = State::Off;
    StateChangedCallback mStateChangedCallback = nullptr;
    float mMeasuredFrequency = 0.0f;
    float mMeasuredRpm = 0;
    float mSpeed = 0.0f;

    // sets voltage on FanEnable pin
    Gpio& mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcChannel& mSpeedPwm;

    // counts tacho pulses on FanTacho pin
    FrequencySensor mFrequencySensor;

    // filter for measured RPM value
    ExponentialFilter mRpmFilter;

    // fan state monitor
    FanStateMonitor mFanStateMonitor;

};

} // namespace