#pragma once

#include "core/hardware/ledc/LedcTimer.h"
#include "core/hardware/ledc/LedcChannel.h"
#include "global/PinConfig.h"

namespace Garbox {

class LedcConfig {

public:

    struct TimerConfig {
        LedcTimer::Id id;
        uint32_t frequencyHz;
        uint8_t resolutionBits;
    };

    struct ChannelConfig {
        LedcTimer::Id timerId;
        LedcChannel::Id channelId;
        uint32_t pin;
        bool invert;
    };

    // Timers config                            TimerId                Freq   Bits
    static constexpr TimerConfig DimmingTimer { LedcTimer::Id::Timer0, 5000,  10 };
    static constexpr TimerConfig FanTimer     { LedcTimer::Id::Timer1, 25000, 8  };

    // Channels config                                 TimerId          ChannelId                  Pin                    Invert
    static constexpr ChannelConfig DebugLed0Channel  { DimmingTimer.id, LedcChannel::Id::Channel0, PinConfig::DebugLed0,  false };
    static constexpr ChannelConfig DebugLed1Channel  { DimmingTimer.id, LedcChannel::Id::Channel1, PinConfig::DebugLed1,  false };
    static constexpr ChannelConfig DebugLed2Channel  { DimmingTimer.id, LedcChannel::Id::Channel2, PinConfig::DebugLed2,  false };
    static constexpr ChannelConfig DebugLed3Channel  { DimmingTimer.id, LedcChannel::Id::Channel3, PinConfig::DebugLed3,  false };
    static constexpr ChannelConfig BacklightChannel  { DimmingTimer.id, LedcChannel::Id::Channel4, PinConfig::DisplayBlk, false };
    static constexpr ChannelConfig FanControlChannel { FanTimer.id,     LedcChannel::Id::Channel5, PinConfig::FanPwm,     true  };

};

} // namespace Garbox
