#pragma once

#include <cstdint>

namespace Garbox {

enum struct ProfilerId : uint8_t {
    MainTask = 0,
    MainTick,
    LogTick,
    DisplayTick,
    PiezoPlayerTick,
    LedAnimationTick,
    Count
};

static constexpr const char* ProfilerIdToString(ProfilerId id){
    switch(id){
        case ProfilerId::MainTask: return "MainTask";
        case ProfilerId::MainTick: return "MainTick";
        case ProfilerId::LogTick: return "LogTick";
        case ProfilerId::DisplayTick: return "DisplayTick";
        case ProfilerId::PiezoPlayerTick: return "PiezoPlayerTick";
        case ProfilerId::LedAnimationTick: return "LedAnimationTick";
        case ProfilerId::Count: return "Count";
    }
    static_assert("unhandled profiler config id");
    return "undefined";
}

} // namespace Garbox
