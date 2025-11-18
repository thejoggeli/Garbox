#pragma once

#include <cstdint>
#include "core/hardware/gpio/Gpio.h"
#include "modules/parts/button/ButtonIfc.h"
#include "modules/parts/button/FsmButton.h"

namespace Garbox {

/**
 * Polling-based Button implementation.
 * 
 * Uses GPIO polling to detect state changes and forwards them to the FsmButton state machine.
 */
class PollingButton : public ButtonIfc {
public:
    PollingButton(Gpio& gpio);

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
    PollingButton(const PollingButton&) = delete;
    PollingButton& operator=(const PollingButton&) = delete;
    PollingButton(PollingButton&&) = delete;
    PollingButton& operator=(PollingButton&&) = delete;

private:

    Gpio& mGpio;
    FsmButton mButton;
    int32_t mPin = -1;

    bool mInitialized = false;
};

} // namespace Garbox
