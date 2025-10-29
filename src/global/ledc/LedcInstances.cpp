#include "LedcInstances.h"

#include "assert/Assert.h"
#include "LedcConfig.h"

namespace Garbox {

static LedcTimer* resolveTimer(LedcTimer::Id id) {
    switch (id) {
        case LedcConfig::DimmingTimer.id: return &LedcInstances::GetDimmingTimer();
        case LedcConfig::FanTimer.id:     return &LedcInstances::GetFanTimer();
        default:
            AssertExit(false, "Invalid LEDC timer ID");
            return nullptr; // never reached
    }
}

static void setupTimer(LedcTimer& timer, LedcConfig::TimerConfig config) {
    timer.setup(config.id, config.frequencyHz, config.resolutionBits);
}

static void setupChannel(LedcChannel& channel, LedcConfig::ChannelConfig config) {
    channel.setup(config.channelId, resolveTimer(config.timerId), config.pin, config.invert);
}

void LedcInstances::Init() {

    // setup timers
    setupTimer(GetDimmingTimer(), LedcConfig::DimmingTimer);
    setupTimer(GetFanTimer(),     LedcConfig::FanTimer);

    // setup channels
    setupChannel(GetDebugLed0Channel(),  LedcConfig::DebugLed0Channel);
    setupChannel(GetDebugLed1Channel(),  LedcConfig::DebugLed1Channel);
    setupChannel(GetDebugLed2Channel(),  LedcConfig::DebugLed2Channel);
    setupChannel(GetDebugLed3Channel(),  LedcConfig::DebugLed3Channel);
    setupChannel(GetBacklightChannel(),  LedcConfig::BacklightChannel);
    setupChannel(GetFanControlChannel(), LedcConfig::FanControlChannel);

}

LedcTimer& LedcInstances::GetDimmingTimer() { static LedcTimer instance; return instance; }
LedcTimer& LedcInstances::GetFanTimer()     { static LedcTimer instance; return instance; }

LedcChannel& LedcInstances::GetDebugLed0Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed1Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed2Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetDebugLed3Channel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetBacklightChannel()  { static LedcChannel instance; return instance; }
LedcChannel& LedcInstances::GetFanControlChannel() { static LedcChannel instance; return instance; }

} // namespace
