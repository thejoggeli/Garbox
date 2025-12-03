#pragma once

#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class LvglContext;

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    void onHeartbeat(const HeartbeatEvent& event) final;

private:

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

};

} // namespace Garbox