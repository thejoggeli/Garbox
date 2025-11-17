#include "LedcInstances.h"

#include "app/config/PinConfig.h"
#include "assert/Assert.h"
#include "LedcConfig.h"

namespace Garbox {

static bool gInitialized = false;

static LedcTimer* resolveTimer(LedcTimer::Id id){
    switch (id){
        case LedcConfig::DimmingTimer.id: return &LedcInstances::GetDimmingTimer();
        case LedcConfig::FanTimer.id:     return &LedcInstances::GetFanTimer();
        case LedcConfig::PiezoTimer.id:   return &LedcInstances::GetPiezoTimer();
        default:
            TriggerExit("LedcInstances", "Invalid LEDC timer ID");
            return nullptr; // never reached
    }
}

static void setupTimer(LedcTimer& timer, LedcConfig::TimerConfig const& config){
    timer.setup(config.id, config.frequencyHz, config.resolutionBits);
}

static void setupChannel(LedcChannel& channel, LedcConfig::ChannelConfig const& config){
    channel.setup(config.channelId, resolveTimer(config.timerId), config.pin, config.invert);
}

void LedcInstances::Init(){

    AssertExit(!gInitialized, "LedInstances", "already initialized");

    // setup timers
    setupTimer(GetDimmingTimer(), LedcConfig::DimmingTimer);
    setupTimer(GetFanTimer(),     LedcConfig::FanTimer);
    setupTimer(GetPiezoTimer(),   LedcConfig::PiezoTimer);

    // setup channels
    setupChannel(GetDebugLed0Channel(),  LedcConfig::DebugLed0Channel);
    setupChannel(GetDebugLed1Channel(),  LedcConfig::DebugLed1Channel);
    setupChannel(GetDebugLed2Channel(),  LedcConfig::DebugLed2Channel);
    setupChannel(GetDebugLed3Channel(),  LedcConfig::DebugLed3Channel);
    setupChannel(GetDisplayBacklightChannel(),  LedcConfig::BacklightChannel);
    setupChannel(GetFanSpeedChannel(), LedcConfig::FanControlChannel);
    setupChannel(GetPiezoChannel(),      LedcConfig::PiezoChannel);

    gInitialized = true;
}

LedcTimer& LedcInstances::GetDimmingTimer(){ static LedcTimer instance; return instance; }
LedcTimer& LedcInstances::GetFanTimer()     { static LedcTimer instance; return instance; }
LedcTimer& LedcInstances::GetPiezoTimer()   { static LedcTimer instance; return instance; }

LedcChannel& LedcInstances::GetDebugLed0Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed1Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed2Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed3Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDisplayBacklightChannel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetFanSpeedChannel(){ static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetPiezoChannel()      { static LedcChannel instance; return instance; }

} // namespace
