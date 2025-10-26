#pragma once

#include <functional>

namespace Garbox {

class AssertHandler final {

public:

    using Handler = std::function<void(const char* message)>;

    static void SetDebugHandler(Handler handler);
    static void SetExitHandler(Handler handler);

    static void InvokeDebug(const char* message);
    static void InvokeExit(const char* message);


private:

    static Handler sDebugHandler;
    static Handler sExitHandler;

};

} // namespace
