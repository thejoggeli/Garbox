#pragma once

#include <cstdint>
#include "core/hardware/gpio/Gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"
#include "modules/parts/button/ButtonIfc.h"
#include "modules/parts/button/FsmButton.h"

namespace Garbox {

/**
 * Interrupt-based Button implementation.
 * 
 * Uses GPIO interrupts to detect state changes and forwards them to the FsmButton state machine.
 */
class InterruptButton : public ButtonIfc {
public:
    InterruptButton(Gpio& gpio);

    void init() final;
    void tick() final;

    // delegate FsmButton API
    bool isPressed() const final;
    bool isReleased() const final;
    bool isLongPressed() const final;

    void setStateChangedCallback(StateChangedCallback callback) final;
    void setHoldCallback(HoldCallback callback) final;
    void setUserData(void* userData) final;

    void setPressedToReleasedDelayMicros(uint32_t micros) final;
    void setReleasedToPressedDelayMicros(uint32_t micros) final;
    void setPressedHoldTimeMicros(uint32_t micros) final;
    void setReleasedHoldTimeMicros(uint32_t micros) final;

    void setLongPressMicros(uint32_t delayMicros) final;
    void setInitialHoldDelayMicros(uint32_t delayMicros) final;
    void setRepeatHoldDelayMicros(uint32_t delayMicros) final;

    // Disallow copy and move 
    InterruptButton(const InterruptButton&) = delete;
    InterruptButton& operator=(const InterruptButton&) = delete;
    InterruptButton(InterruptButton&&) = delete;
    InterruptButton& operator=(InterruptButton&&) = delete;

private:
    static void IRAM_ATTR isrHandler(void* arg);

    Gpio& mGpio;
    FsmButton mButton;

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
