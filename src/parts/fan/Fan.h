#pragma once

#include <cstdint>
#include "FanMonitor.h"
#include "core/sensor/FrequencySensor.h"
#include "util/filter/ExponentialFilter.h"

namespace Garbox {

class Gpio;
class LedcChannel;

class Fan {
public:

    enum class State : uint8_t {
        Disabled = 0,
        Enabled,
        Stalled
    };

    using StateChangedCallback = std::function<void(State newState, State oldState)>;
    using StalledAlertCallback = std::function<void(uint32_t counter)>;

    Fan();

    void init();
    void start();
    void tick();

    void setStateChangedCallback(StateChangedCallback callback);
    void setStalledAlertCallback(StalledAlertCallback callback);
    void setEnabled(bool enabled);
    void setSpeed(float speed); // range [0.0, 1.0]

    bool isEnabled();
    State getState();
    float getSpeed();
    float getMeasuredRpm(bool filtered = true);

    static const char* StateToString(State state);

private:

    void enterState(State state);
    void handleMonitorStateChanged(FanMonitor::State state);
    void handleMonitorStalledAlert(uint32_t counter);

    State mState = State::Disabled;
    StateChangedCallback mStateChangedCallback = nullptr;
    StalledAlertCallback mStalledAlertCallback = nullptr;
    float mMeasuredFrequency = 0.0f;
    float mMeasuredRpm = 0;
    float mMeasuredRpmFiltered = 0;
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
    FanMonitor mFanMonitor;

};

} // namespace