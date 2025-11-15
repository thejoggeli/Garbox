#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace Garbox {

class SystemTasks {
public:

    static void StartAll();
    static void StopAll(TaskHandle_t exclude);

};

} // namespace

