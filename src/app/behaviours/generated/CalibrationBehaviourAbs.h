// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "app/behaviours/generated/BaseBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviourAbs : public BaseBehaviourAbs {
public:

    CalibrationBehaviourAbs(const RuntimeContext& context);

protected:

    // shorthand types for write events 
    using FanCommandWrite = EventWrite<EventPayload::FanCommand>;

    // make typed events
    FanCommandWrite makeFanCommandEvent();

    // send typed events
    void sendEvent(const FanCommandWrite& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEvent;

    // hide unused shorthand types 
    using BaseBehaviourAbs::Heartbeat;
    using BaseBehaviourAbs::FanStatus;
    using BaseBehaviourAbs::FanSample;

    // hide unsued event handlers
    void onButtonRepeat(const ButtonRepeat& event) final;
    void onButtonStateChanged(const ButtonStateChanged& event) final;
    void onEncoderStep(const EncoderStep& event) final;
    void onHeatpadStatus(const HeatpadStatus& event) final;
    void onTemperatureSample(const TemperatureSample& event) final;
    void onTemperatureStatus(const TemperatureStatus& event) final;

};

} // namespace Garbox