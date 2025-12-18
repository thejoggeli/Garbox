#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/TemperatureStatusState.h"
#include "app/states/types/TemperatureSampleState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class I2cPartsControllerAbs : public ControllerAbs {
public:

    // component constructor
    I2cPartsControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onButtonStateChangedEvent(const ButtonStateChangedEvent& event) = 0;

protected:

    // make typed events 
    TemperatureStatusEvent makeTemperatureStatusEvent();
    TemperatureSampleEvent makeTemperatureSampleEvent();

    // send typed events
    void sendEvent(const TemperatureStatusEvent& event);
    void sendEvent(const TemperatureSampleEvent& event);

    // get writable states
    TemperatureStatusState& stateTemperatureStatus();
    TemperatureSampleState& stateTemperatureSample();

private:

    // writable state pointers
    TemperatureStatusState* mTemperatureStatusState = nullptr;
    TemperatureSampleState* mTemperatureSampleState = nullptr;

    // dependency inject writable states
    void injectTemperatureStatusState(TemperatureStatusState* state);
    void injectTemperatureSampleState(TemperatureSampleState* state);

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox