#pragma once

#include <functional>
#include <stdint.h>
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class FanMonitor {
public:
    enum class State : uint8_t {
        Idle,
        Spinning,
        Stalled
    };

    using StateChangedCallback = std::function<void(State state)>;
    using StalledAlertCallback = std::function<void(uint32_t counter)>;

    FanMonitor(
        uint32_t idleStallThresholdMicros = 200000,
        uint32_t spinningStallThresholdMicros = 200000,
        uint32_t stalledAlertPeriodMicros = 0,
        uint32_t minRpmThreshold = 50,
        uint32_t reenterStallCooldownMicros = 0
    );

    void init();
    void setStateChangedCallback(StateChangedCallback callback);
    void setStalledAlertCallback(StalledAlertCallback callback);
    void setStalledAlertPeriod(uint32_t periodMicros);
    void setIdleStallThreshold(uint32_t stallThresholdMicros);
    void setSpinningStallThreshold(uint32_t stallThresholdMicros);
    void setMinRpmThreshold(uint32_t rpmThreshold);
    void setReenterStallCooldown(uint32_t cooldownMicros);
    void reset();
    State getState() const;

    // called from app tick (e.g. 30 Hz)
    void tick(uint32_t rpmValue, bool shouldSpin);

private: 
    uint32_t mIdleStallThresholdMicros; // Idle => Stalled
    uint32_t mSpinningStallThresholdMicros; // Spinning => Stalled
    uint32_t mStalledAlertPeriodMicros;
    uint32_t mReenterStallCooldownMicros;
    uint32_t mMinRpmThreshold;

    bool mInitialized = false;
    State mState = State::Idle;
    StateChangedCallback mStateChangedCallback = nullptr;
    StalledAlertCallback mStalledAlertCallback = nullptr;
    uint32_t mStallCounter = 0;

    SoftwareTimer mStallTimer;
    SoftwareTimer mPeriodicAlertTimer;
    SoftwareTimer mReenterCooldownTimer;

    void enterState(State newState);
    void handleIdleState(uint32_t rpmValue, bool shouldSpin);
    void handleSpinningState(uint32_t rpmValue, bool shouldSpin);
    void handleStalledState(uint32_t rpmValue, bool shouldSpin);

    // state entry and exit helpers
    void onEnterIdleState();
    void onLeaveIdleState();
    void onEnterSpinningState();
    void onLeaveSpinningState();
    void onEnterStalledState();
    void onLeaveStalledState();
};

} // namespace Garbox
