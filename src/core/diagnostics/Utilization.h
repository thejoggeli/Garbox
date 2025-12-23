#pragma once

#include <cstdint>
#include "core/util/function/filter/filters/MovingAverageFilter.h"

namespace Garbox {

class Utilization {
public:

    static void BeginBusy();
    static void EndBusy();
    static void Tick();

    static uint32_t GetBusyMicros();

};

} // namespace
