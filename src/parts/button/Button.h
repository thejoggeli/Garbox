#pragma once

#include <cstdint>
#include <functional>
#include "core/time/SoftwareTimer.h"
#include "util/fsm/FiniteStateMachine.h"

namespace Garbox {

/**
 * Button class using FiniteStateMachine to manage debounce, press, long-press, and hold logic.
 * 
 * Features:
 * - Configurable press/release debounce
 * - Optional long-press detection
 * - Periodic hold callback with initial and repeat delay
 * - All timing in microseconds
 */
class Button {
public:
    enum class State : uint8_t {
        Released,
        Pressed,
        PressedLong,
        Count
    };

    static const char* StateToString(State state);

    using StateChangedCallback = std::function<void(State oldState, State newState, void* userData)>;
    using HoldCallback = std::function<void(uint32_t counter, uint32_t holdTimeMicros, void* userData)>;

    Button();
    ~Button();

    void init();
    void tick(bool isPressedRaw);

    // set callbacks
    void setStateChangedCallback(StateChangedCallback callback);
    void setHoldCallback(HoldCallback callback);
    void setUserData(void* userData);

    // set debouce times
    void setPressedToReleasedDelayMicros(uint32_t micros); // minumum stable signal time before "Pressed => Released" transition is realized
    void setReleasedToPressedDelayMicros(uint32_t micros); // minumum stable signal time before "Released => Pressed" transition is realized
    void setPressedHoldTimeMicros(uint32_t micros);        // minumum time the "Pressed"  state will be held once entered
    void setReleasedHoldTimeMicros(uint32_t micros);       // minumum time the "Released" state will be held once entered

    // set long press delay time
    void setLongPressMicros(uint32_t delayMicros);

    // set initial and repeating hold time
    void setInitialHoldDelayMicros(uint32_t delayMicros);
    void setRepeatHoldDelayMicros(uint32_t delayMicros);

    // handle a missed pulse (e.g. button was pressed and released between two ticks, detected by interrupt)
    void handleMissedPulse(bool isPressedRaw, uint32_t pulseDuration);

    bool isPressed() const;
    bool isLongPressed() const;
    bool isReleased() const;

private:
    FiniteStateMachine<State, State::Count> mFsm;
    SoftwareTimer mHoldTimer;

    StateChangedCallback mStateChangedCallback = nullptr;
    HoldCallback mHoldCallback = nullptr;
    void* mUserData = nullptr;

    bool mInitialized = false;
    uint32_t mLongPressMicros = 0;
    uint32_t mInitialHoldDelayMicros = 0;
    uint32_t mRepeatHoldDelayMicros = 0;

    uint32_t mHoldCounter = 0;
    uint32_t mHoldStartTimeMicros = 0;

    void handleReleasedState(bool isPressedRaw);
    void handlePressedState(bool isPressedRaw);
    void handlePressedLongState(bool isPressedRaw);

    void handleFsmStateChanged(State oldState, State newState);
};

} // namespace Garbox
