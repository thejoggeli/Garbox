// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/behaviour/BehaviourAbs.h"

namespace Garbox {

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    FermentationBehaviourAbs();

    // tick handlers
    virtual void onLogicTick() = 0;

    // shorthand types for read events 
    using Heartbeat = EventRead<EventPayload::Heartbeat>;
    using FanStatus = EventRead<EventPayload::FanStatus>;
    using FanSample = EventRead<EventPayload::FanSample>;
    using HeatpadStatus = EventRead<EventPayload::HeatpadStatus>;
    using TemperatureStatus = EventRead<EventPayload::TemperatureStatus>;
    using TemperatureSample = EventRead<EventPayload::TemperatureSample>;
    using ButtonStateChanged = EventRead<EventPayload::ButtonStateChanged>;
    using ButtonRepeat = EventRead<EventPayload::ButtonRepeat>;
    using EncoderStep = EventRead<EventPayload::EncoderStep>;

    // receive event handlers
    virtual void onHeartbeat(const Heartbeat& event) = 0;
    virtual void onFanStatus(const FanStatus& event) = 0;
    virtual void onFanSample(const FanSample& event) = 0;
    virtual void onHeatpadStatus(const HeatpadStatus& event) = 0;
    virtual void onTemperatureStatus(const TemperatureStatus& event) = 0;
    virtual void onTemperatureSample(const TemperatureSample& event) = 0;
    virtual void onButtonStateChanged(const ButtonStateChanged& event) = 0;
    virtual void onButtonRepeat(const ButtonRepeat& event) = 0;
    virtual void onEncoderStep(const EncoderStep& event) = 0;

protected:

    // shorthand types for write events 
    using FanCommandWrite = EventWrite<EventPayload::FanCommand>;
    using BacklightCommandWrite = EventWrite<EventPayload::BacklightCommand>;
    using HeatpadCommandWrite = EventWrite<EventPayload::HeatpadCommand>;

    // make typed events
    FanCommandWrite makeFanCommandEvent();
    BacklightCommandWrite makeBacklightCommandEvent();
    HeatpadCommandWrite makeHeatpadCommandEvent();

    // send typed events
    void sendEvent(const FanCommandWrite& event);
    void sendEvent(const BacklightCommandWrite& event);
    void sendEvent(const HeatpadCommandWrite& event);

private:

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEvent;

};

} // namespace Garbox