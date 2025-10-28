#pragma once

#include "core/time/SoftwareTimer.h"
#include "parts/display/Display.h"
#include "parts/fan/Fan.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/piezo/Piezo.h"

namespace Garbox {

class MainControl {
public:

    MainControl();

    void init();
    void start();
    void tick();

    void onAssertDebug(const char* message);
    void onAssertExit(const char* message);

private:

    static constexpr uint32_t HeartbeatIntervalMillis = 500;

    Fan mFan;
    Heatpad mHeatpad;
    Display mDisplay;
    Piezo mPiezo;
    
    SoftwareTimer mFanStateTimer;
    SoftwareTimer mHeartbeatTimer;
    SoftwareTimer mRpmTimer;

};

} // namespace