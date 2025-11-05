#pragma once

#include <cstdint>
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

    using StateChangedCallback = void(*)(State oldState, State newState, void* userData);
    using HoldCallback = void(*)(uint32_t counter, uint32_t holdTimeMicros, void* userData);

    Button();
    ~Button();

    void init();
    void tick(bool isPressedRaw);

    void setStateChangedCallback(StateChangedCallback callback);
    void setHoldCallback(HoldCallback callback);
    void setUserData(void* userData);

    void setPressDebounceMicros(uint32_t delayMicros);
    void setReleaseDebounceMicros(uint32_t delayMicros);
    void setLongPressMicros(uint32_t delayMicros);
    void setInitialHoldDelayMicros(uint32_t delayMicros);
    void setRepeatHoldDelayMicros(uint32_t delayMicros);

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
    uint32_t mPressDebounceMicros = 0;
    uint32_t mReleaseDebounceMicros = 0;
    uint32_t mLongPressMicros = 400_ms;
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
