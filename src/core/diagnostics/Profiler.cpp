#include "Profiler.h"
#include <stdlib.h>

namespace Garbox {

Profiler::Record* Profiler::sRecords = nullptr;
uint8_t Profiler::sNumRecords = 0;
uint8_t Profiler::sNextIndex = 0;
bool Profiler::sEnabled = false;
bool Profiler::sInitialized = false;
uint32_t Profiler::sLastUpdateTime = 0;

bool Profiler::Setup(uint8_t num) {
    if (sInitialized) return true;
    if (num == 0) return false;

    sRecords = static_cast<Record*>(malloc(sizeof(Record) * num));
    if (!sRecords) return false;

    sNumRecords = num;
    for (uint8_t i = 0; i < num; ++i) {
        sRecords[i] = {
            0, 0,
            0xFFFFFFFF, 0,   // current min/max
            0xFFFFFFFF, 0,   // last min/max
            0xFFFFFFFF, 0,   // total min/max
            0, false,
            0.0f, 0.0f
        };
    }

    sNextIndex = 0;
    sInitialized = true;
    sLastUpdateTime = Time::GetMicros();
    return true;
}

void Profiler::SetEnabled(bool on) { sEnabled = on; }
bool Profiler::IsEnabled() { return sEnabled; }

void Profiler::Begin(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    Record& r = sRecords[id];
    r.lastBegin = Time::GetMicros();
    r.active = true;
}

void Profiler::End(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    Record& r = sRecords[id];
    if (!r.active) return;
    r.active = false;

    uint32_t now = Time::GetMicros();
    uint32_t duration = now - r.lastBegin;

    // Update current
    if (duration > r.maxDurationCurrent) r.maxDurationCurrent = duration;
    if (duration < r.minDurationCurrent) r.minDurationCurrent = duration;

    // Update total
    if (duration > r.maxDurationTotal) r.maxDurationTotal = duration;
    if (duration < r.minDurationTotal) r.minDurationTotal = duration;

    r.totalTime += duration;
    r.count++;
}

void Profiler::Update(uint8_t id) {
    if (!sInitialized || !sEnabled || id >= sNumRecords) return;
    uint32_t now = Time::GetMicros();
    uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    Record& r = sRecords[id];

    if (r.count > 0) {
        r.avgDuration = static_cast<float>(r.totalTime) / r.count;
        r.frequency   = (r.count * 1e6f) / elapsed;
    } else {
        r.avgDuration = 0.0f;
        r.frequency   = 0.0f;
    }

    // Store current as last, reset current
    r.minDurationLast = r.minDurationCurrent;
    r.maxDurationLast = r.maxDurationCurrent;
    r.minDurationCurrent = 0xFFFFFFFF;
    r.maxDurationCurrent = 0;

    r.count = 0;
    r.totalTime = 0;

    if (id == sNumRecords - 1)
        sLastUpdateTime = now;
}

void Profiler::UpdateAll() {
    if (!sInitialized || !sEnabled) return;
    uint32_t now = Time::GetMicros();
    uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    for (uint8_t i = 0; i < sNumRecords; ++i) {
        Record& r = sRecords[i];

        if (r.count > 0) {
            r.avgDuration = static_cast<float>(r.totalTime) / r.count;
            r.frequency   = (r.count * 1e6f) / elapsed;
        } else {
            r.avgDuration = 0.0f;
            r.frequency   = 0.0f;
        }

        // Move current window to last
        r.minDurationLast = r.minDurationCurrent;
        r.maxDurationLast = r.maxDurationCurrent;

        // Reset current for next window
        r.minDurationCurrent = 0xFFFFFFFF;
        r.maxDurationCurrent = 0;

        r.count = 0;
        r.totalTime = 0;
    }

    sLastUpdateTime = now;
}

const Profiler::Record& Profiler::GetRecord(uint8_t id) {
    static Record dummy = {};
    if (!sInitialized || id >= sNumRecords) return dummy;
    return sRecords[id];
}

const Profiler::Record* Profiler::GetNextRecord() {
    if (!sInitialized || sNumRecords == 0) return nullptr;
    const Record* r = &sRecords[sNextIndex];
    sNextIndex++;
    if (sNextIndex >= sNumRecords) sNextIndex = 0;
    return r;
}

void Profiler::ResetIteration() {
    sNextIndex = 0;
}

void Profiler::ResetTotals(uint8_t id) {
    if (!sInitialized || id >= sNumRecords) return;
    Record& r = sRecords[id];
    r.minDurationTotal = 0xFFFFFFFF;
    r.maxDurationTotal = 0;
}

void Profiler::ResetAllTotals() {
    if (!sInitialized) return;
    for (uint8_t i = 0; i < sNumRecords; ++i) {
        sRecords[i].minDurationTotal = 0xFFFFFFFF;
        sRecords[i].maxDurationTotal = 0;
    }
}

} // namespace Garbox
