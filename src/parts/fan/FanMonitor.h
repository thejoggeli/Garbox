#pragma once

#include <cstdint>
#include "core/time/SoftwareTimer.h"
#include "util/fsm/FiniteStateMachine.h"

namespace Garbox {

class FanMonitor {
public:
    enum class State : uint8_t {
        Idle,
        Spinning,
        Stalled,
        Count
    };

    static const char* StateToString(State state);

    using StateChangedCallback = std::function<void(State oldState, State newState)>;
    using StalledAlertCallback = std::function<void(uint32_t counter)>;

    FanMonitor();

    void init();
    void tick(uint32_t rpmValue, bool shouldSpin);

    void setStateChangedCallback(StateChangedCallback callback);
    void setStalledAlertCallback(StalledAlertCallback callback);

    void setStalledAlertPeriod(uint32_t periodMicros);
    void setMinRpmThreshold(uint32_t rpmThreshold);
    void setTransitionDelay(State from, State to, uint32_t delayMicros);

    State getState() const;

    // Disallow copy and move 
    FanMonitor(const FanMonitor&) = delete;
    FanMonitor& operator=(const FanMonitor&) = delete;
    FanMonitor(FanMonitor&&) = delete;
    FanMonitor& operator=(FanMonitor&&) = delete;

private:
    FiniteStateMachine<State, State::Count> mFsm;
    SoftwareTimer mStalledAlertTimer;

    StateChangedCallback mStateChangedCallback = nullptr;
    StalledAlertCallback mStalledAlertCallback = nullptr;

    bool mInitialized = false;
    uint32_t mStalledAlertPeriodMicros = 0;
    uint32_t mMinRpmThreshold = 0;
    uint32_t mStallCounter = 0;

    void handleIdleState(uint32_t rpmValue, bool shouldSpin);
    void handleSpinningState(uint32_t rpmValue, bool shouldSpin);
    void handleStalledState(uint32_t rpmValue, bool shouldSpin);
    void handleFsmStateChanged(State oldState, State newState);
};

} // namespace Garbox
