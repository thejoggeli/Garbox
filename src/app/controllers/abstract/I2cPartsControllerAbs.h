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

    // state type accessors for getters and setters
    struct TemperatureStatusType {};
    struct TemperatureSampleType {};
    static constexpr TemperatureStatusType TemperatureStatus {};
    static constexpr TemperatureSampleType TemperatureSample {};

    // get writable states
    TemperatureStatusState& writeState(TemperatureStatusType type);
    TemperatureSampleState& writeState(TemperatureSampleType type);

private:

    // writable state pointers
    TemperatureStatusState* mTemperatureStatusState = nullptr;
    TemperatureSampleState* mTemperatureSampleState = nullptr;

    // dependency inject writable states
    void injectWritableState(TemperatureStatusState* state);
    void injectWritableState(TemperatureSampleState* state);

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox