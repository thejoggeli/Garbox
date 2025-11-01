#include "TimerInstances.h"

#include "assert/Assert.h"
#include "global/AppConfig.h"

namespace Garbox {
    
static bool sInitialized = false;

void TimerInstances::Init(){
    AssertExit(!sInitialized, "SpiInstances::Init()", "already initialized");

    GetFanTachoTimer().init({
        .group = TIMER_GROUP_0,
        .index = TIMER_0,
        .frequencyHz = 10'000'000,
        .maxValue = 0, 
        .config = {
            .alarm_en = TIMER_ALARM_DIS,
            .counter_en = TIMER_START,
            .intr_type = TIMER_INTR_LEVEL,
            .counter_dir = TIMER_COUNT_UP,
            .auto_reload = TIMER_AUTORELOAD_EN,
            .divider = 0, // computed from frequencyHz in Timer::init() 
            .clk_src = TIMER_SRC_CLK_APB
        },
    });
    
    sInitialized = true;
}

Timer& TimerInstances::GetFanTachoTimer(){
    static Timer instance;
    return instance;
}

}