#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/controllers/input/InputControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/time/SoftwareTimer.h"
#include "modules/parts/button/ButtonIfc.h"

namespace Garbox {

class ButtonIfc;
class PiezoPlayer;
class AnimatedLed;
class RotaryEncoder;

class InputController : public InputControllerAbs {
private:

    void handleButtonStateChanged(ButtonState oldState, ButtonState newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    void handleEncoderCallback(int32_t steps);
    static void encoderCallbackTrampoline(void* ctx, int32_t steps){
        static_cast<InputController*>(ctx)->handleEncoderCallback(steps);
    }

    AnimatedLed& mButtonStatusLed;
    PiezoPlayer& mPiezoPlayer;
    ButtonIfc& mButton;
    RotaryEncoder& mEncoder;

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    InputController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onSensorReadTick() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace
