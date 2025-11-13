#pragma once

#include <cstdint>
#include "ButtonIfc.h"
#include "PollingButton.h"
#include "core/hardware/gpio/Gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

namespace Garbox {

/**
 * Interrupt-based Button implementation.
 * 
 * Uses GPIO interrupts to detect state changes and forwards them to the PollingButton state machine.
 */
class InterruptButton : ButtonIfc {
public:
    InterruptButton(Gpio& gpio);
    ~InterruptButton();

    void init();
    void tick();

    // delegate PollingButton API
    bool isPressed() const;
    bool isReleased() const;
    bool isLongPressed() const;

    void setStateChangedCallback(ButtonIfc::StateChangedCallback callback);
    void setHoldCallback(ButtonIfc::HoldCallback callback);
    void setUserData(void* userData);

    void setPressedToReleasedDelayMicros(uint32_t micros); // minumum stable signal time before "Pressed => Released" transition is realized
    void setReleasedToPressedDelayMicros(uint32_t micros); // minumum stable signal time before "Released => Pressed" transition is realized
    void setPressedHoldTimeMicros(uint32_t micros);        // minumum time the "Pressed"  state will be held once entered
    void setReleasedHoldTimeMicros(uint32_t micros);       // minumum time the "Released" state will be held once entered

    void setLongPressMicros(uint32_t delayMicros);
    void setInitialHoldDelayMicros(uint32_t delayMicros);
    void setRepeatHoldDelayMicros(uint32_t delayMicros);

private:
    static void IRAM_ATTR isrHandler(void* arg);

    Gpio& mGpio;
    PollingButton mButton;

    int32_t mPin = -1;
    bool mInvert = false;
    bool mCurrentLevelRaw = false;

    volatile bool vIsrReferenceLevelRaw = false;
    volatile bool vIsrPulseStartDetected = false;    
    volatile bool vIsrPulseEndDetected = false;
    volatile uint32_t vIsrPulseStartCpuCycles = 0;
    volatile uint32_t vIsrPulseEndCpuCycles = 0;

    portMUX_TYPE mMux = portMUX_INITIALIZER_UNLOCKED;

    bool mInitialized = false;
};

} // namespace Garbox
