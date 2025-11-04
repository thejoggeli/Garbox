#pragma once

#include <cstdint>

namespace Garbox {

class ProfilerConfig {
public:

    enum Id {
        MainTask = 0,
        MainTick,
        LogTick,
        UiTick,
        PiezoTick,
        Count
    };

    static constexpr const char* IdToString(uint8_t id){
        switch(static_cast<Id>(id)){
            case MainTask: return "MainTask";
            case MainTick: return "MainTick";
            case LogTick: return "LogTick";
            case UiTick: return "UiTick";
            case PiezoTick: return "PiezoTick";
            case Count: return "Count";
        }
        static_assert("unhandled profiler config id");
        return "undefined";
    }

    static constexpr bool EnableProfiler = true;
};

} // namespace Garbox
