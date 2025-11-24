#pragma once

#include <cstdint>

namespace Garbox {

enum struct ProfilerId : uint8_t {
    MainPeriod = 0,
    AllPhases,
    LogTick,
    DisplayTick,
    RenderTick,
    PiezoPlayer,
    LedAnimation,
    Count
};

static constexpr const char* ProfilerIdToString(ProfilerId id){
    switch(id){
        case ProfilerId::MainPeriod: return "MainPeriod";
        case ProfilerId::AllPhases: return "AllPhases";
        case ProfilerId::LogTick: return "LogTick";
        case ProfilerId::DisplayTick: return "DisplayTick";
        case ProfilerId::RenderTick: return "RenderTick";
        case ProfilerId::PiezoPlayer: return "PiezoPlayer";
        case ProfilerId::LedAnimation: return "LedAnimation";
        case ProfilerId::Count: return "Count";
    }
    static_assert("unhandled profiler config id");
    return "undefined";
}

} // namespace Garbox
