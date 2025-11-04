#include "Profiler.h"
#include <stdlib.h>

namespace Garbox {

Profiler::Record* Profiler::sRecords = nullptr;
uint8_t Profiler::sNumRecords = 0;
uint8_t Profiler::sNextIndex = 0;
bool Profiler::sEnabled = false;
bool Profiler::sInitialized = false;
uint32_t Profiler::sLastUpdateTime = 0;
SemaphoreHandle_t Profiler::sMutex = nullptr;

bool Profiler::Setup(uint8_t num) {
    if (sInitialized) return true;
    if (num == 0) return false;

    sRecords = static_cast<Record*>(malloc(sizeof(Record) * num));
    if (!sRecords) return false;

    sMutex = xSemaphoreCreateMutex();
    if (!sMutex) {
        free(sRecords);
        sRecords = nullptr;
        return false;
    }

    sNumRecords = num;
    for (uint8_t i = 0; i < num; ++i) {
        sRecords[i] = {
            0, 0, 0, 0,
            0xFFFFFFFF, 0,
            0xFFFFFFFF, 0,
            0xFFFFFFFF, 0,
            0, false,
            0.0f, 0.0f
        };
    }

    sNextIndex = 0;
    sInitialized = true;
    sLastUpdateTime = Time::GetMicros();
    return true;
}

void Profiler::SetEnabled(bool on) {
    LockGuard lock(sMutex);
    sEnabled = on;
}

bool Profiler::IsEnabled() {
    LockGuard lock(sMutex);
    return sEnabled;
}

void Profiler::Begin(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    LockGuard lock(sMutex);
    Record& r = sRecords[id];
    r.lastBegin = Time::GetMicros();
    r.active = true;
}

void Profiler::End(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    LockGuard lock(sMutex);
    Record& r = sRecords[id];
    if (!r.active) return;
    r.active = false;

    const uint32_t now = Time::GetMicros();
    const uint32_t duration = now - r.lastBegin;

    if (duration > r.maxDurationCurrent) r.maxDurationCurrent = duration;
    if (duration < r.minDurationCurrent) r.minDurationCurrent = duration;
    if (duration > r.maxDurationTotal)   r.maxDurationTotal   = duration;
    if (duration < r.minDurationTotal)   r.minDurationTotal   = duration;

    r.totalTime += duration;
    r.countCurrent++;
    r.countTotal++;
}

void Profiler::ProcessRecord(Record& r, uint32_t elapsed) {
    if (r.countCurrent > 0) {
        r.avgDuration = static_cast<float>(r.totalTime) / r.countCurrent;
        r.frequency   = (r.countCurrent * 1e6f) / elapsed;

        r.minDurationLast = r.minDurationCurrent;
        r.maxDurationLast = r.maxDurationCurrent;
    } else {
        r.avgDuration     = 0.0f;
        r.frequency       = 0.0f;
        r.minDurationLast = 0;
        r.maxDurationLast = 0;
    }

    // Reset interval
    r.minDurationCurrent = 0xFFFFFFFF;
    r.maxDurationCurrent = 0;
    r.countLast          = r.countCurrent;
    r.countCurrent       = 0;
    r.totalTime          = 0;
}

void Profiler::Update(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    LockGuard lock(sMutex);
    const uint32_t now = Time::GetMicros();
    const uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    ProcessRecord(sRecords[id], elapsed);

    if (id == sNumRecords - 1)
        sLastUpdateTime = now;
}

void Profiler::UpdateAll() {
    if (!sInitialized || !sEnabled) return;
    LockGuard lock(sMutex);
    const uint32_t now = Time::GetMicros();
    const uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    for (uint8_t i = 0; i < sNumRecords; ++i)
        ProcessRecord(sRecords[i], elapsed);

    sLastUpdateTime = now;
}

const Profiler::Record& Profiler::GetRecord(uint8_t id) {
    static Record dummy = {};
    if (!sInitialized || id >= sNumRecords) return dummy;
    LockGuard lock(sMutex);
    return sRecords[id];
}

const Profiler::Record* Profiler::GetNextRecord() {
    if (!sInitialized || sNumRecords == 0) return nullptr;
    LockGuard lock(sMutex);
    const Record* r = &sRecords[sNextIndex];
    sNextIndex++;
    if (sNextIndex >= sNumRecords) sNextIndex = 0;
    return r;
}

void Profiler::ResetIteration() {
    LockGuard lock(sMutex);
    sNextIndex = 0;
}

void Profiler::ResetTotals(uint8_t id) {
    if (!sInitialized || id >= sNumRecords) return;
    LockGuard lock(sMutex);
    Record& r = sRecords[id];
    r.minDurationTotal = 0xFFFFFFFF;
    r.maxDurationTotal = 0;
}

void Profiler::ResetAllTotals() {
    if (!sInitialized) return;
    LockGuard lock(sMutex);
    for (uint8_t i = 0; i < sNumRecords; ++i) {
        sRecords[i].minDurationTotal = 0xFFFFFFFF;
        sRecords[i].maxDurationTotal = 0;
    }
}

} // namespace Garbox
