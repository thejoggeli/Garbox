#pragma once

#include <functional>
#include "core/rtos/Task.h"

namespace Garbox {

class TaskManager {
public:

    using StopHandler = std::function<void()>;

    // register task + handler for emergency stop
    static void RegisterStopHandler(StopHandler handler);

    // starts all system tasks
    static void StartAll();

    // Requests every managed task to stop. Each external task is joined with a
    // bound; the calling task is request-only and may remain as fatal reporter.
    static void StopAll();

};

} // namespace
