#pragma once

#include <cstdint>
#include <cstring>
#include <functional>
#include "assert/Assert.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

/**
 * Generic finite state machine supporting delayed transitions and state hold times.
 */
class FiniteStateMachine {
public:
    using StateChangedCallback = std::function<void(uint8_t oldState, uint8_t newState)>;

    FiniteStateMachine(uint8_t numStates);
    ~FiniteStateMachine();

    // initialize with initial optional state
    void init(uint8_t initialState = 0);

    // must be called periodically to handle TransitionDelay and StateHoldTime timers
    void tick();

    // delay between two states before the transition is realized (useful for debouncing)
    // e.g. useful to only transition between two state if a signal remains stable for a minimum time
    void setTransitionDelayMicros(uint8_t from, uint8_t to, uint32_t delayMicros);

    // minimum time to stay in a state before any other transition is possible (useful for debouncing)
    // e.g. useful to debounce a button press/release, so it says in the new state for a minimum time
    void setStateHoldTimeMicros(uint8_t state, uint32_t holdMicros);

    // register callback for (oldState, newState)
    void setStateChangedCallback(StateChangedCallback callback);

    // schedules a delayed transition, respecting TransitionDelay and StateHoldTime
    void transition(uint8_t newState);

    // immediate transition, ignoring hold/delay constraints
    void forceTransition(uint8_t newState);

    // check if there is a pending delayed transition
    bool hasPendingTransition();

    // cancel the pending delayed transition
    void cancelPendingTransition();

    // current state
    uint8_t getState() const { return mCurrentState; }

private:

    const uint8_t mNumStates;
    uint8_t mCurrentState = 0;
    uint8_t mPendingState = 0;

    uint32_t** mTransitionDelayMicros = nullptr; // transition delay matrix
    uint32_t* mStateHoldTimeMicros = nullptr; // state hold time vector

    SoftwareTimer mTransitionTimer;
    SoftwareTimer mStateHoldTimer;

    StateChangedCallback mStateChangedCallback = nullptr;
    bool mInitialized = false;

    void applyTransition(uint8_t newState);
};

} // namespace Garbox
