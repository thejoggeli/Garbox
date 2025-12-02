#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/behaviour/BehaviourAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    FermentationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeat(const HeartbeatEvent& event) = 0;

    virtual void onFanStatus(const FanStatusEvent& event) = 0;

    virtual void onFanSample(const FanSampleEvent& event) = 0;

    virtual void onHeatpadStatus(const HeatpadStatusEvent& event) = 0;

    virtual void onTemperatureStatus(const TemperatureStatusEvent& event) = 0;

    virtual void onTemperatureSample(const TemperatureSampleEvent& event) = 0;

    virtual void onButtonStateChanged(const ButtonStateChangedEvent& event) = 0;

    virtual void onButtonRepeat(const ButtonRepeatEvent& event) = 0;

    virtual void onEncoderStep(const EncoderStepEvent& event) = 0;

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

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::publishEvent;

};

} // namespace Garbox