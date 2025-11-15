#pragma once

#include <cstdint>
#include <functional>
#include "core/time/SoftwareTimer.h"
#include "parts/button/ButtonIfc.h"
#include "util/fsm/FiniteStateMachine.h"

namespace Garbox {

/**
 * ButtonIfc implementation using FiniteStateMachine to manage debounce, press, long-press, and hold logic.
 */
class FsmButton : public ButtonIfc{
public:

    using StateChangedCallback = std::function<void(ButtonState oldState, ButtonState newState, void* userData)>;
    using HoldCallback = std::function<void(uint32_t counter, uint32_t holdTimeMicros, void* userData)>;

    FsmButton();
    ~FsmButton();

    void init() final;
    void tick() final;

    // set callbacks
    void setStateChangedCallback(StateChangedCallback callback) final;
    void setHoldCallback(HoldCallback callback) final;
    void setUserData(void* userData) final;

    // set debouce times
    void setPressedToReleasedDelayMicros(uint32_t micros) final;
    void setReleasedToPressedDelayMicros(uint32_t micros) final;
    void setPressedHoldTimeMicros(uint32_t micros) final;
    void setReleasedHoldTimeMicros(uint32_t micros) final;

    // set long press delay time
    void setLongPressMicros(uint32_t delayMicros) final;

    // set initial and repeating hold time
    void setInitialHoldDelayMicros(uint32_t delayMicros) final;
    void setRepeatHoldDelayMicros(uint32_t delayMicros) final;

    bool isPressed() const final;
    bool isLongPressed() const final;
    bool isReleased() const final;

    // set the raw button state
    void setPhysicalButtonState(bool pressed);

    // handle a missed pulse (e.g. button was pressed and released between two ticks, detected by interrupt)
    void handleMissedPulse(uint32_t pulseDuration);

    // Disallow copy and move 
    FsmButton(const FsmButton&) = delete;
    FsmButton& operator=(const FsmButton&) = delete;
    FsmButton(FsmButton&&) = delete;
    FsmButton& operator=(FsmButton&&) = delete;

private:

    FiniteStateMachine<ButtonState, ButtonState::Count> mFsm;
    SoftwareTimer mHoldTimer;

    bool mIsPressedPhysical;

    StateChangedCallback mStateChangedCallback = nullptr;
    HoldCallback mHoldCallback = nullptr;
    void* mUserData = nullptr;

    bool mInitialized = false;
    uint32_t mLongPressMicros = 0;
    uint32_t mInitialHoldDelayMicros = 0;
    uint32_t mRepeatHoldDelayMicros = 0;

    uint32_t mHoldCounter = 0;
    uint32_t mHoldStartTimeMicros = 0;

    void handleReleasedState();
    void handlePressedState();
    void handlePressedLongState();

    void handleFsmStateChanged(ButtonState oldState, ButtonState newState);
};

} // namespace Garbox
