#pragma once

#include <cstdint>
#include "FanMonitor.h"
#include "core/sensor/FrequencySensor.h"
#include "global/AppConfig.h"
#include "util/filter/MovingAverageFilter.h"

namespace Garbox {

class Gpio;
class LedcChannel;

class Fan {
public:

    enum class State : uint8_t {
        Disabled = 0,
        Enabled,
        Stalled,
        Count
    };

    static const char* StateToString(State state);

    using StateChangedCallback = std::function<void(State oldState, State newState)>;
    using StalledAlertCallback = std::function<void(uint32_t counter)>;

    Fan();

    void init();
    void start();
    void tick();

    void setStateChangedCallback(StateChangedCallback callback);
    void setStalledAlertCallback(StalledAlertCallback callback);

    void setSpeed(float speed); // range [0.0, 1.0]
    void setEnabled(bool enabled);

    bool isEnabled() const;
    bool isStalled() const;
    State getState() const;
    float getSpeed() const;
    uint32_t getMeasuredRpm(bool filtered = true) const;

private:

    using MonitorState = FanMonitor::State;

    void handleMonitorStateChanged(MonitorState oldState, MonitorState newState);
    void handleMonitorStalledAlert(uint32_t counter);

    void updateState();

    State mState = State::Disabled;
    bool mEnabled = false;
    bool mStalled = false;
    StateChangedCallback mStateChangedCallback = nullptr;
    StalledAlertCallback mStalledAlertCallback = nullptr;
    float mSpeed = 0.0f;
    float mMeasuredFrequency = 0.0f;
    uint32_t mMeasuredRpm = 0;
    uint32_t mMeasuredRpmFiltered = 0;

    // sets voltage on FanEnable pin
    Gpio& mGpioFanEnable;

    // sends pwm signal to FanPwm pin
    LedcChannel& mSpeedPwm;

    // counts tacho pulses on FanTacho pin
    FrequencySensor mFrequencySensor;

    // filter for measured RPM value
    static constexpr size_t RpmFilterSize = std::max(1u, AppConfig::MainTaskFrequencyHz/3);
    MovingAverageFilter<uint32_t, RpmFilterSize> mRpmFilter;

    // fan state monitor
    FanMonitor mMonitor;

};

} // namespace