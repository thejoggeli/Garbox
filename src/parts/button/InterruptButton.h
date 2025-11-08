#pragma once

#include <cstdint>
#include "Button.h"
#include "core/hardware/gpio/Gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

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

    void setPressDebounceMicros(uint32_t debounceMicros);
    void setReleaseDebounceMicros(uint32_t debounceMicros);
    void setLongPressMicros(uint32_t delayMicros);
    void setInitialHoldDelayMicros(uint32_t delayMicros);
    void setRepeatHoldDelayMicros(uint32_t delayMicros);

private:
    static void IRAM_ATTR isrHandler(void* arg);

    Gpio& mGpio;
    Button mButton;

    int32_t mPin = -1;
    bool mInvert = false;

    bool mCurrentRawState = false;
    volatile uint32_t vLastEdgeTimeMicros = 0;
    volatile uint32_t vCurrentEdgeTimeMicros = 0;
    volatile bool vNewRawState = false;
    volatile bool vEdgeDetected = false;    
    portMUX_TYPE mMux = portMUX_INITIALIZER_UNLOCKED;

    bool mInitialized = false;
};

} // namespace Garbox
