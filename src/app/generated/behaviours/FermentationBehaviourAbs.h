#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/generated/states/types/FermentationStatusState.h"
#include "app/generated/states/types/FanSampleState.h"
#include "app/generated/states/types/FanStatusState.h"
#include "app/generated/states/types/TemperatureSampleState.h"
#include "app/generated/states/types/TemperatureStatusState.h"

#include "core/application/behaviour/BehaviourAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class Runtime;

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    FermentationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() {};

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) {};
    virtual void onButtonStateChangedEvent(const ButtonStateChangedEvent& event) {};
    virtual void onButtonRepeatEvent(const ButtonRepeatEvent& event) {};
    virtual void onEncoderStepEvent(const EncoderStepEvent& event) {};

    // state change handlers (to be implmeneted by user)
    virtual void onFanSampleStateChanged(const FanSampleState& state) {};
    virtual void onFanStatusStateChanged(const FanStatusState& state) {};
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) {};
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) {};

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();
    DisplayCommandEvent makeDisplayCommandEvent();
    HeatpadCommandEvent makeHeatpadCommandEvent();
    FermentationStatusEvent makeFermentationStatusEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);
    void sendEvent(const DisplayCommandEvent& event);
    void sendEvent(const HeatpadCommandEvent& event);
    void sendEvent(const FermentationStatusEvent& event);

    // state access struct
    class States final {
    public:

        States(
            FermentationStatusState& fermentationStatusState, // write
            const FanSampleState& fanSampleState, // read
            const FanStatusState& fanStatusState, // read
            const TemperatureSampleState& temperatureSampleState, // read
            const TemperatureStatusState& temperatureStatusState // read
        ):
            fermentationStatus(fermentationStatusState),
            fanSample(fanSampleState),
            fanStatus(fanStatusState),
            temperatureSample(temperatureSampleState),
            temperatureStatus(temperatureStatusState){
        }

        // disallow copy and move
        States(const States&) = delete;
        States& operator=(const States&) = delete;
        States(States&&) = delete;
        States& operator=(States&&) = delete;

        // writable states
        FermentationStatusState& fermentationStatus;

        // readable states
        const FanSampleState& fanSample;
        const FanStatusState& fanStatus;
        const TemperatureSampleState& temperatureSample;
        const TemperatureStatusState& temperatureStatus;
    };

    States& states();

private:

    std::optional<States> mStates;

    void bindStates(
        FermentationStatusState& fermentationStatus,
        const FanSampleState& fanSample,
        const FanStatusState& fanStatus,
        const TemperatureSampleState& temperatureSample,
        const TemperatureStatusState& temperatureStatus
    );

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox