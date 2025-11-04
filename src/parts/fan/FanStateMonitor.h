#pragma once

#include <functional>
#include <stdint.h>
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class FanStateMonitor {
public:
    enum class State : uint8_t {
        Off,
        On,
        Stalled
    };

    using Callback = std::function<void(State state)>;

    FanStateMonitor(
        uint32_t stallThresholdMicros = 200_ms,
        uint32_t stalledCallbackPeriodMicros = 0,
        uint32_t minRpmThreshold = 50,
        uint32_t reenterStallCooldownMicros = 0
    );

    void init();
    void setCallback(Callback callback);
    void setStalledCallbackPeriod(uint32_t periodMicros);
    void setStallThreshold(uint32_t stallThresholdMicros);
    void setMinRpmThreshold(uint32_t rpmThreshold);
    void setReenterStallCooldown(uint32_t cooldownMicros);
    void reset();
    State getState() const;

    // called from app tick (e.g. 30 Hz)
    void tick(uint32_t rpmValue, bool shouldRun);

private:
    uint32_t mStallThresholdMicros;
    uint32_t mStalledCallbackPeriodMicros;
    uint32_t mReenterStallCooldownMicros;
    uint32_t mMinRpmThreshold;
    
    bool mInitialized = false;
    State mState = State::Off;
    Callback mCallback = nullptr;

    SoftwareTimer mStallTimer;
    SoftwareTimer mPeriodicStallTimer;
    SoftwareTimer mReenterCooldownTimer;

    void enterState(State newState);
    void handleOffState(uint32_t rpmValue, bool shouldRun);
    void handleOnState(uint32_t rpmValue, bool shouldRun);
    void handleStalledState(uint32_t rpmValue, bool shouldRun);

    // state entry and exit helpers
    void onEnterOffState();
    void onLeaveOffState();
    void onEnterOnState();
    void onLeaveOnState();
    void onEnterStalledState();
    void onLeaveStalledState();
};

} // namespace Garbox
