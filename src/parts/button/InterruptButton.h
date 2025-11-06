#pragma once

#include <cstdint>
#include "Button.h"
#include "core/hardware/gpio/Gpio.h"

namespace Garbox {

/**
 * Interrupt-based Button wrapper.
 * 
 * Uses GPIO interrupts to detect state changes and forwards
 * them to the Button state machine.
 */
class InterruptButton {
public:
    InterruptButton(Gpio& gpio);
    ~InterruptButton();

    void init();
    void tick();

    // delegate Button API
    bool isPressed() const;
    bool isReleased() const;
    bool isLongPressed() const;

    void setStateChangedCallback(Button::StateChangedCallback callback);
    void setHoldCallback(Button::HoldCallback callback);
    void setUserData(void* userData);

    void setPressDebounceMicros(uint32_t delayMicros);
    void setReleaseDebounceMicros(uint32_t delayMicros);
    void setLongPressMicros(uint32_t delayMicros);
    void setInitialHoldDelayMicros(uint32_t delayMicros);
    void setRepeatHoldDelayMicros(uint32_t delayMicros);

private:
    static void IRAM_ATTR isrHandler(void* arg);

    Gpio& mGpio;
    Button mButton;
    int32_t mPin;

    volatile bool mLastRawState = false;
    volatile bool mEdgeDetected = false;
    
    bool mInitialized = false;
};

} // namespace Garbox
