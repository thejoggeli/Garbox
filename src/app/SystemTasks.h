#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace Garbox {

class SystemTasks {
public:

    // starts all system tasks
    static void StartAll();

    // stops all system tasks, excluding the task that is calling this function
    static void StopAll();

};

} // namespace
