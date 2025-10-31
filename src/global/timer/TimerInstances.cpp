#include "TimerInstances.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"
#include "TimerConfig.h"

namespace Garbox {

void TimerInstances::Init(){
    GetFanTachoTimer().init(TimerConfig::FanTachoTimer.cfg);
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