#include "DevtoolsController.h"

#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/util/helpers/StringUtils.h"

namespace Garbox {

DevtoolsController::DevtoolsController() : DevtoolsControllerAbs(){
    // nothing to do
}

void DevtoolsController::onInit(){
    // nothing to do
}

void DevtoolsController::onStart(){
    mProfilerTimer.start(30_s);
}

void DevtoolsController::onLoggingTick(){
    if(mProfilerTimer.isExpired()){
        logProfiler();
        mProfilerTimer.restart();
    }   
}

void DevtoolsController::logProfiler(){
    Profiler::UpdateAll();
    uint32_t seconds = Time::GetSeconds();
    static char timeStringBuffer[20];
    StringUtils::FormatDurationDHMS(seconds, timeStringBuffer, sizeof(timeStringBuffer));
    LogInfo("Dev", "===================== Diagnostics %s =====================", timeStringBuffer);
    LogInfo("Dev", " | ProfilerId    | Count | freq(Hz) | min(us) | avg(us) | max(us) |");
    for (uint32_t i = 0; i < static_cast<uint32_t>(ProfilerId::Count); ++i){
        const ProfilerId id = static_cast<ProfilerId>(i);
        const Profiler::Record& r = Profiler::GetRecord(id);
        const char* idStr = ProfilerIdToString(id);
        LogInfo("Dev", " | %-13.13s | %5" PRIu32 " | %8.3f | %7" PRIu32 " | %7.0f | %7" PRIu32 " |", idStr, r.countLast, r.frequency, r.minDurationLast, r.avgDuration, r.maxDurationLast);
    }
    LogInfo("Dev", "====================================================================");
}

} // namespace Garbox