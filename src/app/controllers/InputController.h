#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "modules/parts/button/ButtonIfc.h"

namespace Garbox {

class ButtonIfc;
class PiezoPlayer;
class AnimatedLed;

class InputController : public ControllerAbs {
public:

    InputController(ComponentId id);

    void onTick();

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
