#include "LedcInstances.h"

#include "core/assert/Assert.h"


namespace Garbox {

using TimerId = LedcTimer::Id;
using ChannelId = LedcChannel::Id;

static bool sInitialized = false;

void LedcInstances::Init(){

    AssertExit(!sInitialized, "LedcInstances", "already initialized");

    // setup timers
    GetDimmingTimer().init(TimerId::Timer0, 5000, 10);
    GetFanSpeedTimer().init(TimerId::Timer1, 25000, 8);
    GetPiezoTimer().init(TimerId::Timer2, 1000, 6);

    // setup channels
    GetStatusLed0Channel().init(ChannelId::Channel0, &GetDimmingTimer(), PinStatusLed0Channel, false);
    GetStatusLed1Channel().init(ChannelId::Channel1, &GetDimmingTimer(), PinStatusLed1Channel, false);
    GetStatusLed2Channel().init(ChannelId::Channel2, &GetDimmingTimer(), PinStatusLed2Channel, false);
    GetStatusLed3Channel().init(ChannelId::Channel3, &GetDimmingTimer(), PinStatusLed3Channel, false);
    GetBacklightChannel().init(ChannelId::Channel4, &GetDimmingTimer(), PinBacklightChannel, false);
    GetFanSpeedChannel().init(ChannelId::Channel5, &GetFanSpeedTimer(), PinFanSpeedChannel, true);
    GetPiezoChannel().init(ChannelId::Channel6, &GetPiezoTimer(), PinPiezoChannel, false);

    sInitialized = true;
}

LedcTimer& LedcInstances::GetDimmingTimer(){
    static LedcTimer instance;
    return instance;
}
LedcTimer& LedcInstances::GetFanSpeedTimer(){
    static LedcTimer instance;
    return instance;
}
LedcTimer& LedcInstances::GetPiezoTimer(){
    static LedcTimer instance;
    return instance;
}

LedcChannel& LedcInstances::GetStatusLed0Channel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetStatusLed1Channel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetStatusLed2Channel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetStatusLed3Channel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetBacklightChannel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetFanSpeedChannel(){
    static LedcChannel instance;
    return instance;
}
LedcChannel& LedcInstances::GetPiezoChannel(){
    static LedcChannel instance;
    return instance;
}

} // namespace