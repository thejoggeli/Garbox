// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ProfilerId.h"

namespace Garbox {

const char* ProfilerIdToString(ProfilerId id){
    switch(id){
        case ProfilerId::Null: return "Null";
        case ProfilerId::MainPeriod: return "MainPeriod";
        case ProfilerId::MainBusy: return "MainBusy";
        case ProfilerId::HeartbeatTick: return "HeartbeatTick";
        case ProfilerId::InputTick: return "InputTick";
        case ProfilerId::LogicTick: return "LogicTick";
        case ProfilerId::OutputTick: return "OutputTick";
        case ProfilerId::LoggingTick: return "LoggingTick";
        case ProfilerId::RenderTick: return "RenderTick";
        case ProfilerId::LedAnimation: return "LedAnimation";
        case ProfilerId::PiezoPlayer: return "PiezoPlayer";
        case ProfilerId::DisplayRender: return "DisplayRender";
        case ProfilerId::Runtime1: return "Runtime1";
        case ProfilerId::Runtime2: return "Runtime2";
        case ProfilerId::Runtime3: return "Runtime3";
        case ProfilerId::Count: return "Count";
    }
    return "Invalid";
}

} // namespace Garbox