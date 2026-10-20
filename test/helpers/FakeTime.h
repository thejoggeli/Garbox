#pragma once

#include <cstdint>

namespace GarboxTest::FakeTime {

void Reset(uint64_t micros = 0);
void SetMicros(uint64_t micros);
void AdvanceMicros(uint64_t micros);
uint64_t GetMicros64();

} // namespace GarboxTest::FakeTime
