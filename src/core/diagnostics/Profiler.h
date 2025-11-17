#pragma once

#include <array>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <stdint.h>
#include <stddef.h>

#include "app/types/ProfilerId.h"
#include "core/time/Time.h"
#include "util/threading/LockGuard.h"

namespace Garbox {

class Profiler {
public:

    // Scoped RAII profiler
    struct Scoped {
        Scoped(ProfilerId id);
        ~Scoped();
    private:
        ProfilerId mId;
    }; 
    
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

    static bool Setup();
    static void Start();
    static void SetEnabled(bool on);
    static bool IsEnabled();

    static void Begin(ProfilerId id);
    static void End(ProfilerId id);
    static void Update(ProfilerId id);
    static void UpdateAll();

    static const Record& GetRecord(ProfilerId id);
    static const Record* GetNextRecord();
    static void ResetIteration();

    static void ResetTotals(ProfilerId id);
    static void ResetAllTotals();

private:
    static void ProcessRecord(Record& r, uint32_t elapsed);
};

} // namespace Garbox
