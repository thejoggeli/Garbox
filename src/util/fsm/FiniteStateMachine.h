#pragma once

#include <cstdint>
#include <functional>
#include "core/time/SoftwareTimer.h"

namespace Garbox {

/**
 * Generic finite state machine supporting delayed transitions and state hold times.
 */
template<typename StateType, StateType NumStates>
class FiniteStateMachine {
public:

    using StateChangedCallback = std::function<void(StateType oldState, StateType newState)>;

    FiniteStateMachine();
    ~FiniteStateMachine();

    // initialize with initial optional state
    void init(StateType initialState = 0);

    // must be called periodically to handle TransitionDelay and StateHoldTime timers
    void tick();

    // delay between two states before the transition is realized (useful for debouncing)
    // e.g. useful to only transition between two state if a signal remains stable for a minimum time
    void setTransitionDelayMicros(StateType from, StateType to, uint32_t delayMicros);

    // minimum time to stay in a state before any other transition is possible (useful for debouncing)
    // e.g. useful to debounce a button press/release, so it says in the new state for a minimum time
    void setStateHoldTimeMicros(StateType state, uint32_t holdMicros);

    // register callback for (oldState, newState)
    void setStateChangedCallback(StateChangedCallback callback);

    // schedules a delayed transition, respecting TransitionDelay and StateHoldTime
    void transition(StateType newState);

    // immediate transition, ignoring hold/delay constraints
    void forceTransition(StateType newState);

    // check if there is a pending delayed transition
    bool hasPendingTransition();

    // cancel the pending delayed transition
    void cancelPendingTransition();

    // current state
    StateType getState() const { return mCurrentState; }

private:

    StateType mCurrentState = static_cast<StateType>(0);
    StateType mPendingState = static_cast<StateType>(0);

    uint32_t mTransitionDelayMicros[static_cast<size_t>(NumStates)][static_cast<size_t>(NumStates)] = {}; // transition delay matrix
    uint32_t mStateHoldTimeMicros[static_cast<size_t>(NumStates)] = {}; // state hold time vector

    SoftwareTimer mTransitionTimer;
    SoftwareTimer mStateHoldTimer;

    StateChangedCallback mStateChangedCallback = nullptr;
    bool mInitialized = false;

    void applyTransition(StateType newState);
};

} // namespace Garbox

#include "FiniteStateMachine.tpp"
