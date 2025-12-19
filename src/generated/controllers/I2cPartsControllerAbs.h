#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"

#include "core/application/controller/ControllerAbs.h"

namespace Garbox {

class Runtime;

class I2cPartsControllerAbs : public ControllerAbs {
public:

    // component constructor
    I2cPartsControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onButtonStateChangedEvent(const ButtonStateChangedEvent& event) {};

protected:

    // state access struct
    class States final {
    public:

        States(
            TemperatureStatusState& temperatureStatusState, // write
            TemperatureSampleState& temperatureSampleState // write
        ):
            temperatureStatus(temperatureStatusState),
            temperatureSample(temperatureSampleState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        TemperatureStatusState& temperatureStatus;
        TemperatureSampleState& temperatureSample;

    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        TemperatureStatusState& temperatureStatus,
        TemperatureSampleState& temperatureSample
    );

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox