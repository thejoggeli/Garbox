#include "Profiler.h"

#include "core/assert/Assert.h"

namespace Garbox {

static constexpr size_t RecordsCount = static_cast<size_t>(ProfilerId::Count);
static std::array<Profiler::Record, RecordsCount> sRecords = {};
static uint8_t sNextIndex = 0;
static bool sEnabled = false;
static bool sInitialized = false;
static uint32_t sLastUpdateTime = 0;
static SemaphoreHandle_t sMutex = nullptr;

ProfilerScoped::ProfilerScoped(ProfilerId id) : mId(id){ 
    Profiler::Begin(mId); 
}

ProfilerScoped::~ProfilerScoped(){ 
    Profiler::End(mId); 
}

bool Profiler::Setup(){
    AssertExit(!sInitialized, "Profiler", "already initialized");
    AssertExit(RecordsCount != 0, "Profiler", "no records"); 

    sMutex = xSemaphoreCreateMutex();
    AssertExit(sMutex != nullptr, "Profiler", "failed to create mutex");

    for (uint8_t i = 0; i < RecordsCount; ++i){
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

void Profiler::Start(){
    AssertExit(sInitialized, "Profiler", "not initialized");
    LockGuard lock(sMutex);
    sLastUpdateTime = Time::GetMicros();
    sEnabled = true;

    // Optionally reset per-frame counters so first tick is clean
    for (uint8_t i = 0; i < RecordsCount; ++i){
        sRecords[i].countCurrent = 0;
        sRecords[i].totalTime = 0;
        sRecords[i].minDurationCurrent = 0xFFFFFFFF;
        sRecords[i].maxDurationCurrent = 0;
    }
}

void Profiler::SetEnabled(bool on){
    LockGuard lock(sMutex);
    sEnabled = on;
}

bool Profiler::IsEnabled(){
    LockGuard lock(sMutex);
    return sEnabled;
}

void Profiler::Begin(ProfilerId id){
    if (!sEnabled) return;
    AssertExit(sInitialized, "Profiler", "not initialized");
    AssertExit(static_cast<uint8_t>(id) < RecordsCount, "Profiler", "invalid id");
    LockGuard lock(sMutex);
    Record& r = sRecords[static_cast<uint8_t>(id)];
    r.lastBegin = Time::GetMicros();
    r.active = true;
}

void Profiler::End(ProfilerId id){
    if (!sEnabled) return;
    AssertExit(sInitialized, "Profiler", "not initialized");
    AssertExit(static_cast<uint8_t>(id) < RecordsCount, "Profiler", "invalid id");
    LockGuard lock(sMutex);
    Record& r = sRecords[static_cast<uint8_t>(id)];
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

void Profiler::ProcessRecord(Record& r, uint32_t elapsed){
    if (r.countCurrent > 0){
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

void Profiler::Update(ProfilerId id){
    if (!sEnabled) return;
    AssertExit(sInitialized, "Profiler", "not initialized");
    AssertExit(static_cast<uint8_t>(id) < RecordsCount, "Profiler", "invalid id");
    LockGuard lock(sMutex);
    const uint32_t now = Time::GetMicros();
    const uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    ProcessRecord(sRecords[static_cast<uint8_t>(id)], elapsed);

    if (static_cast<uint8_t>(id) == RecordsCount - 1)
        sLastUpdateTime = now;
}

void Profiler::UpdateAll(){
    if (!sEnabled) return;
    AssertExit(sInitialized, "Profiler", "not initialized");
    LockGuard lock(sMutex);
    const uint32_t now = Time::GetMicros();
    const uint32_t elapsed = now - sLastUpdateTime;
    if (elapsed == 0) return;

    for (uint8_t i = 0; i < RecordsCount; ++i)
        ProcessRecord(sRecords[i], elapsed);

    sLastUpdateTime = now;
}

const Profiler::Record& Profiler::GetRecord(ProfilerId id){
    AssertExit(sInitialized, "Profiler", "not initialized");
    AssertExit(static_cast<uint8_t>(id) < RecordsCount, "Profiler", "invalid id");
    LockGuard lock(sMutex);
    return sRecords[static_cast<uint8_t>(id)];
}

const Profiler::Record* Profiler::GetNextRecord(){
    AssertExit(sInitialized, "Profiler", "not initialized");
    LockGuard lock(sMutex);
    const Record* r = &sRecords[sNextIndex];
    sNextIndex++;
    if (sNextIndex >= RecordsCount) sNextIndex = 0;
    return r;
}

void Profiler::ResetIteration(){
    LockGuard lock(sMutex);
    sNextIndex = 0;
}

void Profiler::ResetTotals(ProfilerId id){
    AssertExit(sInitialized, "Profiler", "not initialized");
    AssertExit(static_cast<uint8_t>(id) < RecordsCount, "Profiler", "invalid id");
    LockGuard lock(sMutex);
    Record& r = sRecords[static_cast<uint8_t>(id)];
    r.minDurationTotal = 0xFFFFFFFF;
    r.maxDurationTotal = 0;
}

void Profiler::ResetAllTotals(){
    AssertExit(sInitialized, "Profiler", "not initialized");
    LockGuard lock(sMutex);
    for (uint8_t i = 0; i < RecordsCount; ++i){
        sRecords[i].minDurationTotal = 0xFFFFFFFF;
        sRecords[i].maxDurationTotal = 0;
    }
}

} // namespace Garbox
