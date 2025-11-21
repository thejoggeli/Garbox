#pragma once

#include "app/controllers/generated/InputControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "modules/parts/button/ButtonIfc.h"

namespace Garbox {

class ButtonIfc;
class PiezoPlayer;
class AnimatedLed;

class InputController : public InputControllerAbs {
public:

    InputController(ComponentId id, const RuntimeContext& context);

    void onInputTick() final;

private:

    void onInit() final;
    void onStart() final;

    void handleButtonStateChanged(ButtonState oldState, ButtonState newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    AnimatedLed& mButtonStatusLed;
    PiezoPlayer& mPiezoPlayer;
    ButtonIfc& mButton;
};

} // namespace
