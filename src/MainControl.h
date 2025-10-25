#pragma once

#include "core/Timer.h"
#include "fan/Fan.h"

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

    static constexpr uint32_t FanStateIntervalMillis = 5000; 
    static constexpr uint32_t HeartbeatIntervalMillis = 500;

    Fan mFan;
    Timer mFanStateTimer;
    Timer mHeartbeatTimer;

};

} // namespace