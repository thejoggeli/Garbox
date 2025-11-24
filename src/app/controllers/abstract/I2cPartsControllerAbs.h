#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class I2cPartsControllerAbs : public ControllerAbs {
public:
    
    I2cPartsControllerAbs();

    // tick handlers
    virtual void onInputTick() = 0;

    // receive event handlers
    virtual void onButtonStateChanged(const ButtonStateChangedEvent& event) = 0;

protected:

    // make typed events
    TemperatureStatusEvent makeTemperatureStatusEvent();
    TemperatureSampleEvent makeTemperatureSampleEvent();

    // send typed events
    void sendEvent(const TemperatureStatusEvent& event);
    void sendEvent(const TemperatureSampleEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox