#pragma once

#include <cstdint>
#include <functional>
#include "core/time/TimeLiterals.h"

namespace Garbox {

/**
 * Simple interface for button.
 * Supports press, long-press, and hold logic.
 * 
 * Features:
 * - Configurable press/release debounce
 * - Optional long-press detection
 * - Periodic hold callback with initial and repeat delay
 * - All timing in microseconds
 */
class ButtonIfc {
protected:

    static constexpr uint32_t InitialPressedToReleasedDelayMicros = 1_ms; // Pressed     state will be entered after a 1ms stable "pressed" signal
    static constexpr uint32_t InitialReleasedToPressedDelayMicros = 1_ms; // Released    state will be entered after a 1ms stable "released" signal
    static constexpr uint32_t InitialPressedHoldTimeMicros = 10_ms;       // Pressed     state will be held for at least 10ms (debouncing) 
    static constexpr uint32_t InitialReleasedHoldTimeMicros = 40_ms;      // Released    state will be held for at least 40ms (debouncing) 
    static constexpr uint32_t InitialLongPressMicros = 600_ms;            // LongPressed state will be entered after 600ms 

public:


    enum class State : uint8_t {
        Released,
        Pressed,
        PressedLong,
        Count
    };

    inline static const char* StateToString(State state){
        switch(state){
        case State::Released: return "Released";
        case State::Pressed: return "Pressed";
        case State::PressedLong: return "PressedLong";
        default: return "Unknown";
        }
    }

    using StateChangedCallback = std::function<void(State oldState, State newState, void* userData)>;
    using HoldCallback = std::function<void(uint32_t counter, uint32_t holdTimeMicros, void* userData)>;

    virtual void init() = 0;
    virtual void tick() = 0;

    // set callbacks
    virtual void setStateChangedCallback(StateChangedCallback callback) = 0;
    virtual void setHoldCallback(HoldCallback callback) = 0;
    virtual void setUserData(void* userData) = 0;

    // set debouce times
    virtual void setPressedToReleasedDelayMicros(uint32_t micros) = 0; // minumum stable signal time before "Pressed => Released" transition is realized
    virtual void setReleasedToPressedDelayMicros(uint32_t micros) = 0; // minumum stable signal time before "Released => Pressed" transition is realized
    virtual void setPressedHoldTimeMicros(uint32_t micros) = 0;        // minumum time the "Pressed"  state will be held once entered
    virtual void setReleasedHoldTimeMicros(uint32_t micros) = 0;       // minumum time the "Released" state will be held once entered

    // set long press delay time
    virtual void setLongPressMicros(uint32_t delayMicros) = 0;

    // set initial and repeating hold time
    virtual void setInitialHoldDelayMicros(uint32_t delayMicros) = 0;
    virtual void setRepeatHoldDelayMicros(uint32_t delayMicros) = 0;

    virtual bool isPressed() const = 0;
    virtual bool isLongPressed() const = 0;
    virtual bool isReleased() const = 0;

};

} // namespace Garbox
