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

    // Timers config                                   TimerId                Freq   Bits
    inline static constexpr TimerConfig DimmingTimer { LedcTimer::Id::Timer0, 5000,  10 };
    inline static constexpr TimerConfig FanTimer     { LedcTimer::Id::Timer1, 25000, 8  };
    inline static constexpr TimerConfig PiezoTimer   { LedcTimer::Id::Timer2, 1000,  6  };

    // Channels config                                        TimerId          ChannelId                  Pin                    Invert
    inline static constexpr ChannelConfig DebugLed0Channel  { DimmingTimer.id, LedcChannel::Id::Channel0, PinConfig::DebugLed0,  false };
    inline static constexpr ChannelConfig DebugLed1Channel  { DimmingTimer.id, LedcChannel::Id::Channel1, PinConfig::DebugLed1,  false };
    inline static constexpr ChannelConfig DebugLed2Channel  { DimmingTimer.id, LedcChannel::Id::Channel2, PinConfig::DebugLed2,  false };
    inline static constexpr ChannelConfig DebugLed3Channel  { DimmingTimer.id, LedcChannel::Id::Channel3, PinConfig::DebugLed3,  false };
    inline static constexpr ChannelConfig BacklightChannel  { DimmingTimer.id, LedcChannel::Id::Channel4, PinConfig::DisplayBlk, false };
    inline static constexpr ChannelConfig FanControlChannel { FanTimer.id,     LedcChannel::Id::Channel5, PinConfig::FanPwm,     true  };
    inline static constexpr ChannelConfig PiezoChannel      { PiezoTimer.id,   LedcChannel::Id::Channel6, PinConfig::Piezo,      false };

};

} // namespace Garbox
