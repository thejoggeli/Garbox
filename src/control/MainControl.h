#pragma once

#include "core/time/SoftwareTimer.h"
#include "parts/button/InterruptButton.h"
#include "parts/display/Display.h"
#include "parts/fan/Fan.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/piezo/PiezoPlayer.h"

namespace Garbox {

class MainControl {
public:

    MainControl();

    void init();
    void start();
    void tick();

    void onAssertDebug(const char* context, const char* message);
    void onAssertExit(const char* context, const char* message);

private:

    void handleButtonStateChanged(Button::State oldState, Button::State newState);
    void handleButtonHold(uint32_t counter, uint32_t holdTimeMicros);

    void handleFanStateChanged(Fan::State oldState, Fan::State newState);
    void handleFanStalledAlert(uint32_t counter);

    static constexpr uint32_t HeartbeatInterval = 500_ms;

    Fan mFan;
    Heatpad mHeatpad;
    Display mDisplay;
    PiezoPlayer mPiezoPlayer;
    InterruptButton mButton;
    
    SoftwareTimer mFanStateTimer;
    SoftwareTimer mHeartbeatTimer;
    SoftwareTimer mRpmTimer;


    bool mInitialized;

};

} // namespace