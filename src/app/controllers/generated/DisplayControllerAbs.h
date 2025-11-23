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
    virtual void onTemperatureStatus(const TemperatureStatusEvent& event) = 0;
    virtual void onTemperatureSample(const TemperatureSampleEvent& event) = 0;
    virtual void onBacklightCommand(const BacklightCommandEvent& event) = 0;
    virtual void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox