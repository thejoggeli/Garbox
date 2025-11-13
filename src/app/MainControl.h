#pragma once

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

class MainControl {
public:

    MainControl();

    void init();
    void start();
    void tick();

    void onAssertDebug(const char* context, const char* message);
    void onAssertExit(const char* context, const char* message);

private:

    void handleButtonStateChanged(ButtonIfc::State oldState, ButtonIfc::State newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    void handleFanStateChanged(Fan::State oldState, Fan::State newState);
    void handleFanStalledAlert(uint32_t counter);

    RgbLed& mRgbLed;
    StatusLeds& mStatusLeds;
    AnimatedLed& mHeartbeatLed;
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

    bool mInitialized;

};

} // namespace