#pragma once

// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/event/EventView.h"
#include "shared/types/EventType.h"

namespace Garbox {

// generated from config
using ActiveBehaviourChangedEvent = EventView<EventType::ActiveBehaviourChanged>;
using HeartbeatEvent              = EventView<EventType::Heartbeat>;
using BacklightCommandEvent       = EventView<EventType::BacklightCommand>;
using FanStatusEvent              = EventView<EventType::FanStatus>;
using FanSampleEvent              = EventView<EventType::FanSample>;
using FanCommandEvent             = EventView<EventType::FanCommand>;
using HeatpadStatusEvent          = EventView<EventType::HeatpadStatus>;
using HeatpadCommandEvent         = EventView<EventType::HeatpadCommand>;
using TemperatureStatusEvent      = EventView<EventType::TemperatureStatus>;
using TemperatureSampleEvent      = EventView<EventType::TemperatureSample>;
using ButtonStateChangedEvent     = EventView<EventType::ButtonStateChanged>;
using ButtonRepeatEvent           = EventView<EventType::ButtonRepeat>;
using EncoderStepEvent            = EventView<EventType::EncoderStep>;

} // namespace Garbox