// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class I2cPartsControllerAbs : public ControllerAbs {
public:
    
    // controller constructor
    I2cPartsControllerAbs(const RuntimeContext& context);

    // tick handlers
    virtual void onInputTick() = 0;

    // receive event handlers
    virtual void onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event) = 0;

protected:

    // make typed events
    EventWrite<EventPayload::TemperatureStatus> makeTemperatureStatusEvent();
    EventWrite<EventPayload::TemperatureSample> makeTemperatureSampleEvent();

    // send typed events
    void sendEvent(const EventWrite<EventPayload::TemperatureStatus>& event);
    void sendEvent(const EventWrite<EventPayload::TemperatureSample>& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox