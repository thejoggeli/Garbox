#pragma once

#include "core/hardware/timer/Timer.h"

namespace Garbox {

class TimerInstances {
public:

    static void Init();

    static Timer& GetFanTacho();

};

} // namespace Garbox
