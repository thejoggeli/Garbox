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

class InputController : public InputControllerAbs {
private:

    void handleButtonStateChanged(ButtonState oldState, ButtonState newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    AnimatedLed& mButtonStatusLed;
    PiezoPlayer& mPiezoPlayer;
    ButtonIfc& mButton;

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    InputController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onInputTick() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace
