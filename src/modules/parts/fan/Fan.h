#pragma once

#include <cstdint>
#include "modules/parts/fan/FanState.h"
#include "modules/parts/fan/FanMonitor.h"
#include "modules/sensors/FrequencySensor.h"
#include "core/util/conditioner/TachoConditioner.h"

namespace Garbox {

class Gpio;
class LedcChannel;

class Fan {
public:

    struct Config {
        Gpio& enableGpio; 
        LedcChannel& speedPwm; 
        Gpio& tachoGpio;
        Timer& tachoTimer;
        uint32_t tachoPulsesPerRev = 1;
        uint32_t tachoFilterTicks = 10;
    };

    using StateChangedCallback = std::function<void(FanState oldState, FanState newState)>;
    using StalledAlertCallback = std::function<void(uint32_t counter)>;

    Fan(const Config& config);

    void init();
    void start();
    void tick();

    void setStateChangedCallback(StateChangedCallback callback);
    void setStalledAlertCallback(StalledAlertCallback callback);

    void setTargetSpeed(float speed); // range [0.0, 1.0]
    void setEnabled(bool enabled);

    bool isEnabled() const;
    bool isStalled() const;
    FanState getState() const;
    float getTargetSpeed() const;
    float getMeasuredRpm(bool filtered = true) const;

    // disallow copy and move 
    Fan(const Fan&) = delete;
    Fan& operator=(const Fan&) = delete;
    Fan(Fan&&) = delete;
    Fan& operator=(Fan&&) = delete;

private:

    using MonitorState = FanMonitor::State;

    void handleMonitorStateChanged(MonitorState oldState, MonitorState newState);
    void handleMonitorStalledAlert(uint32_t counter);

    void updateState();

    bool mInitialized = false;

    FanState mState = FanState::Disabled;
    bool mEnabled = false;
    bool mStalled = false;
    StateChangedCallback mStateChangedCallback = nullptr;
    StalledAlertCallback mStalledAlertCallback = nullptr;
    float mTargetSpeed = 0.0f;

    // sets voltage on FanEnable pin
    Gpio& mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcChannel& mSpeedPwm;

    // counts tacho pulses on FanTacho pin
    FrequencySensor mFrequencySensor;

    // filter for measured RPM value
    TachoConditioner mTachoConditioner;
    const uint32_t mTachoPulsesPerRev;
    const float mHzToRpmFactor;

    // fan state monitor
    FanMonitor mMonitor;

};

} // namespace