#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/event/EventView.h"
#include "shared/types/EventPayload.h"
#include "shared/types/EventType.h"

namespace Garbox {

// event view aliases
using HeartbeatEvent              = EventView<EventType::Heartbeat>;
using DisplayCommandEvent         = EventView<EventType::DisplayCommand>;
using FanCommandEvent             = EventView<EventType::FanCommand>;
using HeatpadCommandEvent         = EventView<EventType::HeatpadCommand>;
using TemperatureSampleEvent      = EventView<EventType::TemperatureSample>;
using ButtonStateChangedEvent     = EventView<EventType::ButtonStateChanged>;
using ButtonRepeatEvent           = EventView<EventType::ButtonRepeat>;
using EncoderStepEvent            = EventView<EventType::EncoderStep>;
using ActiveBehaviourChangedEvent = EventView<EventType::ActiveBehaviourChanged>;
using ActiveScreenChangedEvent    = EventView<EventType::ActiveScreenChanged>;

// event payload aliases
using HeartbeatPayload              = EventPayload<EventType::Heartbeat>;
using DisplayCommandPayload         = EventPayload<EventType::DisplayCommand>;
using FanCommandPayload             = EventPayload<EventType::FanCommand>;
using HeatpadCommandPayload         = EventPayload<EventType::HeatpadCommand>;
using TemperatureSamplePayload      = EventPayload<EventType::TemperatureSample>;
using ButtonStateChangedPayload     = EventPayload<EventType::ButtonStateChanged>;
using ButtonRepeatPayload           = EventPayload<EventType::ButtonRepeat>;
using EncoderStepPayload            = EventPayload<EventType::EncoderStep>;
using ActiveBehaviourChangedPayload = EventPayload<EventType::ActiveBehaviourChanged>;
using ActiveScreenChangedPayload    = EventPayload<EventType::ActiveScreenChanged>;

} // namespace Garbox