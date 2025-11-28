#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
public:

    MainScreenAbs();

    // receive event handlers
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    // update handlers
    void updateFanSample(const FanSamplePayload& payload);
    void updateFanStatus(const FanStatusPayload& payload);
    void updateHeatpadSample(const HeatpadSamplePayload& payload);
    void updateHeatpadStatus(const HeatpadStatusPayload& payload);

protected:

    // make typed events
    BacklightCommandEvent makeBacklightCommandEvent();

    // send typed events
    void sendEvent(const BacklightCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

};

} // namespace Garbox