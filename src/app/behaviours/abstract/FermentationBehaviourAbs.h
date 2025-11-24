#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    FermentationBehaviourAbs();

    // tick handlers
    virtual void onLogicTick() = 0;

    // receive event handlers
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
    BacklightCommandEvent makeBacklightCommandEvent();
    HeatpadCommandEvent makeHeatpadCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);
    void sendEvent(const BacklightCommandEvent& event);
    void sendEvent(const HeatpadCommandEvent& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::publishEvent;

};

} // namespace Garbox