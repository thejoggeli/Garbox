#pragma once

#include <functional>

namespace Garbox {

class AssertHandler final {

public:

    using Handler = std::function<void(const char* context, const char* message, int32_t arg)>;

    // Configure handlers during single-threaded startup, before worker tasks start.
    static void SetDebugHandler(Handler handler);
    static void SetExitHandler(Handler handler);

    // Task-context APIs. Handlers may log, block, stop tasks, and touch devices;
    // they are intentionally not ISR-safe.
    static void InvokeDebug(const char* context, const char* message, int32_t arg);
    static void InvokeExit(const char* context, const char* message, int32_t arg);

};

} // namespace
