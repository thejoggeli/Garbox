#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "parts/button/ButtonIfc.h"
#include "parts/fan/Fan.h"

namespace Garbox {

class ButtonIfc;
class Display;
class Fan;
class Heatpad;
class PiezoPlayer;

class AnimatedLed;
class RgbLed;
class StatusLeds;

class GarboxController : public ControllerAbs {
public:

    GarboxController();
    void onDisplayTick();

private:

    void onInit() final;
    void onStart() final;
    void onTick() final;

    void handleButtonStateChanged(ButtonState oldState, ButtonState newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    void handleFanStateChanged(Fan::State oldState, Fan::State newState);
    void handleFanStalledAlert(uint32_t counter);

    RgbLed& mRgbLed;
    StatusLeds& mStatusLeds;
    AnimatedLed& mFanStatusLed;
    AnimatedLed& mButtonStatusLed;

    Fan& mFan;
    Heatpad& mHeatpad;
    Display& mDisplay;
    PiezoPlayer& mPiezoPlayer;
    ButtonIfc& mButton;

    SoftwareTimer mFanStateTimer;
    SoftwareTimer mHeartbeatTimer;
    SoftwareTimer mRpmTimer;

};

} // namespace
