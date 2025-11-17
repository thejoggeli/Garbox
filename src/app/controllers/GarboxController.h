#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "parts/button/ButtonIfc.h"

namespace Garbox {

class ButtonIfc;
class PiezoPlayer;
class AnimatedLed;

class GarboxController : public ControllerAbs {
public:

    GarboxController();

private:

    void onInit() final;
    void onStart() final;
    void onTick() final;

    void handleButtonStateChanged(ButtonState oldState, ButtonState newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    AnimatedLed& mButtonStatusLed;
    PiezoPlayer& mPiezoPlayer;
    ButtonIfc& mButton;
};

} // namespace
