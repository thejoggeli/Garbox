#include "TimerInstances.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"
#include "TimerConfig.h"

namespace Garbox {

void TimerInstances::Init(){
    GetFanTachoTimer().init(TimerConfig::FanTachoTimer.cfg, TimerConfig::FanTachoTimer.maxValue);
}

Timer& TimerInstances::GetFanTachoTimer(){
    static Timer instance (
        TimerConfig::FanTachoTimer.group, 
        TimerConfig::FanTachoTimer.idx, 
        TimerConfig::FanTachoTimer.frequencyHz
    );
    return instance;
}

}