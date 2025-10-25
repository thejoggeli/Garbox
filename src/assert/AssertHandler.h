#pragma once

#include <functional>

namespace Garbox {

class AssertHandler final {

public:

    using Handler = std::function<void(const char* message)>;

    static void Init();
    static void Start();

    // Non-fatal (DEBUG) handler
    static void InvokeDebug(const char* message);

    // Fatal (EXIT) handler
    static void InvokeExit(const char* message);


private:

    static Handler sExitHandler;
    static Handler sDebugHandler;

};

} // namespace
