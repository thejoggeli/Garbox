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
    I2cPartsControllerAbs();

    // tick handlers
    virtual void onInputTick() = 0;

    // shorthand types for read events 
    using ButtonStateChanged = EventRead<EventPayload::ButtonStateChanged>;

    // receive event handlers
    virtual void onButtonStateChanged(const ButtonStateChanged& event) = 0;

protected:

    // shorthand types for write events 
    using TemperatureStatusWrite = EventWrite<EventPayload::TemperatureStatus>;
    using TemperatureSampleWrite = EventWrite<EventPayload::TemperatureSample>;

    // make typed events
    TemperatureStatusWrite makeTemperatureStatusEvent();
    TemperatureSampleWrite makeTemperatureSampleEvent();

    // send typed events
    void sendEvent(const TemperatureStatusWrite& event);
    void sendEvent(const TemperatureSampleWrite& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEvent;

};

} // namespace Garbox