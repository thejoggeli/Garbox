#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DisplayControllerAbs : public ControllerAbs {
public:
    
    DisplayControllerAbs();

    // tick handlers
    virtual void onRenderTick() = 0;

    // receive event handlers
    virtual void onFanStatus(const FanStatusEvent& event) = 0;
    virtual void onFanSample(const FanSampleEvent& event) = 0;
    virtual void onHeatpadStatus(const HeatpadStatusEvent& event) = 0;
    virtual void onHeatpadSample(const HeatpadSampleEvent& event) = 0;
    virtual void onTemperatureStatus(const TemperatureStatusEvent& event) = 0;
    virtual void onTemperatureSample(const TemperatureSampleEvent& event) = 0;
    virtual void onDisplayCommand(const DisplayCommandEvent& event) = 0;
    virtual void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) = 0;
    virtual void onFermentationStatus(const FermentationStatusEvent& event) = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox