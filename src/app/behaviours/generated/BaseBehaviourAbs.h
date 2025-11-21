// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#pragma once

#include "core/application/behaviour/BehaviourAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class BaseBehaviourAbs : public BehaviourAbs {
public:
    
    // behaviour constructor
    BaseBehaviourAbs(ComponentId id, const RuntimeContext& context);

    // tick handlers
    virtual void onLogicTick() = 0;

    // shorthand types for read events 
    using ButtonRepeat = EventRead<EventPayload::ButtonRepeat>;
    using ButtonStateChanged = EventRead<EventPayload::ButtonStateChanged>;
    using EncoderStep = EventRead<EventPayload::EncoderStep>;
    using FanSample = EventRead<EventPayload::FanSample>;
    using FanStatus = EventRead<EventPayload::FanStatus>;
    using Heartbeat = EventRead<EventPayload::Heartbeat>;
    using HeatpadStatus = EventRead<EventPayload::HeatpadStatus>;
    using TemperatureSample = EventRead<EventPayload::TemperatureSample>;
    using TemperatureStatus = EventRead<EventPayload::TemperatureStatus>;

    // receive event handlers
    virtual void onButtonRepeat(const ButtonRepeat& event) = 0;
    virtual void onButtonStateChanged(const ButtonStateChanged& event) = 0;
    virtual void onEncoderStep(const EncoderStep& event) = 0;
    virtual void onFanSample(const FanSample& event) = 0;
    virtual void onFanStatus(const FanStatus& event) = 0;
    virtual void onHeartbeat(const Heartbeat& event) = 0;
    virtual void onHeatpadStatus(const HeatpadStatus& event) = 0;
    virtual void onTemperatureSample(const TemperatureSample& event) = 0;
    virtual void onTemperatureStatus(const TemperatureStatus& event) = 0;

};

} // namespace Garbox