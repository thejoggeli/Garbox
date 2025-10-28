#pragma once

#include "core/SoftwareTimer.h"
#include "display/Display.h"
#include "fan/Fan.h"
#include "heatpad/Heatpad.h"
#include "piezo/Piezo.h"

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