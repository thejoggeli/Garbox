#pragma once

#include <functional>

namespace Garbox {

class AssertHandling final {

public:

    using Handler = std::function<void(const char* context, const char* message, int32_t arg)>;

    static void SetDebugHandler(Handler handler);
    static void SetExitHandler(Handler handler);

    static void InvokeDebug(const char* context, const char* message, int32_t arg);
    static void InvokeExit(const char* context, const char* message, int32_t arg);

};

} // namespace
