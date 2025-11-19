#pragma once

#include <functional>
#include "core/rtos/Task.h"

namespace Garbox {

class SystemTasks {
public:

    using StopHandler = std::function<void()>;

    // register task + handler for emergency stop
    static void RegisterStopHandler(StopHandler handler);

    // starts all system tasks
    static void StartAll();

    // stops all system tasks, excluding the task that is calling this function
    static void StopAll();

};

} // namespace
