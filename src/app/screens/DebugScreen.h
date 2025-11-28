#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    void onUpdateScreen() final;

    void onHeartbeat(const HeartbeatEvent& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox