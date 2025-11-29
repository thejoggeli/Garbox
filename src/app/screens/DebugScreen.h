#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class LvglObjects;

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    void onUpdateScreen() final;

    void onHeartbeat(const HeartbeatEvent& event) final;

private:

    LvglObjects& mObjects;
    bool mFirstUpdate = true;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox