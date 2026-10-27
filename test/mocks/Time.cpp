#include "core/time/Time.h"

#include "helpers/FakeTime.h"

namespace {

uint64_t gMicros = 0;
uint32_t gTickMicros = 0;
uint64_t gTickMicros64 = 0;
uint32_t gTickMillis = 0;
uint32_t gTickSeconds = 0;
uint32_t gTickDeltaMicros = 0;
float gTickDeltaSeconds = 0.0f;

} // namespace

namespace GarboxTest::FakeTime {

void Reset(uint64_t micros){
    gMicros = micros;
    gTickMicros = static_cast<uint32_t>(micros);
    gTickMicros64 = micros;
    gTickMillis = static_cast<uint32_t>(micros / 1000ULL);
    gTickSeconds = static_cast<uint32_t>(micros / 1000000ULL);
    gTickDeltaMicros = 0;
    gTickDeltaSeconds = 0.0f;
}

void SetMicros(uint64_t micros){
    gMicros = micros;
}

void AdvanceMicros(uint64_t micros){
    gMicros += micros;
}

uint64_t GetMicros64(){
    return gMicros;
}

} // namespace GarboxTest::FakeTime

namespace Garbox {

void Time::Start(){
    gTickMicros64 = GetMicros64();
    gTickMicros = static_cast<uint32_t>(gTickMicros64);
    gTickMillis = static_cast<uint32_t>(gTickMicros64 / 1000ULL);
    gTickSeconds = static_cast<uint32_t>(gTickMicros64 / 1000000ULL);
    gTickDeltaMicros = 0;
    gTickDeltaSeconds = 0.0f;
}

void Time::Tick(){
    const uint64_t nowMicros64 = GetMicros64();
    const uint32_t nowMicros = static_cast<uint32_t>(nowMicros64);
    gTickDeltaMicros = nowMicros - gTickMicros;
    gTickDeltaSeconds = static_cast<float>(gTickDeltaMicros) * 1e-6f;
    gTickMicros = nowMicros;
    gTickMicros64 = nowMicros64;
    gTickMillis = static_cast<uint32_t>(nowMicros64 / 1000ULL);
    gTickSeconds = static_cast<uint32_t>(nowMicros64 / 1000000ULL);
}

uint32_t Time::GetMicros(){
    return static_cast<uint32_t>(gMicros);
}

uint64_t Time::GetMicros64(){
    return gMicros;
}

uint32_t Time::GetMillis(){
    return static_cast<uint32_t>(gMicros / 1000ULL);
}

uint32_t Time::GetSeconds(){
    return static_cast<uint32_t>(gMicros / 1000000ULL);
}

uint32_t Time::GetCpuCycles(){
    return static_cast<uint32_t>(gMicros * 240ULL);
}

uint32_t Time::CpuCyclesToMicros(uint32_t cycles){
    return cycles / 240U;
}

uint32_t Time::GetTickMicros(){
    return gTickMicros;
}

uint64_t Time::GetTickMicros64(){
    return gTickMicros64;
}

uint32_t Time::GetTickMillis(){
    return gTickMillis;
}

uint32_t Time::GetTickSeconds(){
    return gTickSeconds;
}

uint32_t Time::GetTickDeltaMicros(){
    return gTickDeltaMicros;
}

float Time::GetTickDeltaSeconds(){
    return gTickDeltaSeconds;
}

void Time::DelayMillis(uint32_t millis){
    GarboxTest::FakeTime::AdvanceMicros(static_cast<uint64_t>(millis) * 1000ULL);
}

void Time::BlockMillis(uint32_t millis){
    GarboxTest::FakeTime::AdvanceMicros(static_cast<uint64_t>(millis) * 1000ULL);
}

void Time::BlockMicros(uint32_t micros){
    GarboxTest::FakeTime::AdvanceMicros(micros);
}

} // namespace Garbox
