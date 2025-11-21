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

    // receive event handlers
    virtual void onButtonRepeat(const EventRead<EventPayload::ButtonRepeat>& event) = 0;
    virtual void onButtonStateChanged(const EventRead<EventPayload::ButtonStateChanged>& event) = 0;
    virtual void onEncoderStep(const EventRead<EventPayload::EncoderStep>& event) = 0;
    virtual void onFanSample(const EventRead<EventPayload::FanSample>& event) = 0;
    virtual void onFanStatus(const EventRead<EventPayload::FanStatus>& event) = 0;
    virtual void onHeartbeat(const EventRead<EventPayload::Heartbeat>& event) = 0;
    virtual void onHeatpadStatus(const EventRead<EventPayload::HeatpadStatus>& event) = 0;
    virtual void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) = 0;
    virtual void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) = 0;

};

} // namespace Garbox