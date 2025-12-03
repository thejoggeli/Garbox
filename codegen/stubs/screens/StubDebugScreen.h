#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    // event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

private:

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; 

};

} // namespace Garbox