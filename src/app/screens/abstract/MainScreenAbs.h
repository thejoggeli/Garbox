#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
public:

    MainScreenAbs();

    // receive event handlers
    virtual void onFanStatus(const FanStatusEvent& event) = 0;
    virtual void onFanSample(const FanSampleEvent& event) = 0;
    virtual void onHeatpadStatus(const HeatpadStatusEvent& event) = 0;
    virtual void onHeatpadSample(const HeatpadSampleEvent& event) = 0;
    virtual void onTemperatureStatus(const TemperatureStatusEvent& event) = 0;
    virtual void onTemperatureSample(const TemperatureSampleEvent& event) = 0;
    virtual void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) = 0;
    virtual void onFermentationStatus(const FermentationStatusEvent& event) = 0;
    virtual void onDisplayStatus(const DisplayStatusEvent& event) = 0;

protected:

    // make typed events
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

};

} // namespace Garbox