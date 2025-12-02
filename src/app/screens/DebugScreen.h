#pragma once

#include "app/screens/abstract/DebugScreenAbs.h"

namespace Garbox {

class LvglContext;

class DebugScreen : public DebugScreenAbs {
public:

    DebugScreen();

    void onHeartbeat(const HeartbeatEvent& event) final;

private:

    LvglContext& mContext;
    bool mFirstUpdate = true;

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; // safe to update ui elements in this method

};

} // namespace Garbox