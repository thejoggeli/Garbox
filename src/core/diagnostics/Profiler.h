#pragma once
#include <stdint.h>
#include <stddef.h>
#include "core/time/Time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "global/config/ProfilerConfig.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

class Profiler {
public:
    struct Record {
        uint32_t countCurrent;
        uint32_t countLast;
        uint32_t countTotal;
        uint32_t totalTime;

        uint32_t minDurationCurrent;
        uint32_t maxDurationCurrent;
        uint32_t minDurationLast;
        uint32_t maxDurationLast;
        uint32_t minDurationTotal;
        uint32_t maxDurationTotal;

        uint32_t lastBegin;
        bool active;

        float frequency;
        float avgDuration;
    };

    // Scoped RAII profiler section
    struct Scoped {
        explicit Scoped(uint8_t id) : mId(id) { Profiler::Begin(mId); }
        ~Scoped() { Profiler::End(mId); }

        Scoped(const Scoped&) = delete;
        Scoped& operator=(const Scoped&) = delete;
    private:
        uint8_t mId;
    };

    static bool Setup(uint8_t num);
    static void Start();
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
    static void ProcessRecord(Record& r, uint32_t elapsed);

    static Record* sRecords;
    static uint8_t sNumRecords;
    static uint8_t sNextIndex;
    static bool sEnabled;
    static bool sInitialized;
    static uint32_t sLastUpdateTime;
    static SemaphoreHandle_t sMutex;
};

} // namespace Garbox
