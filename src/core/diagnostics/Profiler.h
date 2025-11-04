#pragma once

#include <stdint.h>
#include <stddef.h>
#include "core/time/Time.h"
#include "global/ProfilerConfig.h"

namespace Garbox {

class Profiler {
public:
    struct Record {
        uint32_t count;
        uint32_t totalTime;

        // Interval tracking
        uint32_t minDurationCurrent;
        uint32_t maxDurationCurrent;

        // Last completed window
        uint32_t minDurationLast;
        uint32_t maxDurationLast;

        // Global since startup
        uint32_t minDurationTotal;
        uint32_t maxDurationTotal;

        uint32_t lastBegin;
        bool active;

        float frequency;     // executions per second during last update
        float avgDuration;   // average duration (µs) during last update
    };

    static bool Setup(uint8_t num);
    static void SetEnabled(bool on);
    static bool IsEnabled();

    static void Begin(uint8_t id);
    static void End(uint8_t id);
    static void Update(uint8_t id);
    static void UpdateAll();

    static const Record& GetRecord(uint8_t id);
    static const Record* GetNextRecord();
    static void ResetIteration();

    static void ResetTotals(uint8_t id);
    static void ResetAllTotals();

private:
    static Record* sRecords;
    static uint8_t sNumRecords;
    static uint8_t sNextIndex;
    static bool sEnabled;
    static bool sInitialized;
    static uint32_t sLastUpdateTime;
};

} // namespace Garbox
