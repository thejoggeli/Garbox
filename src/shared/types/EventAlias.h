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
using FermentationStatusEvent     = EventView<EventType::FermentationStatus>;
using BacklightCommandEvent       = EventView<EventType::BacklightCommand>;
using FanStatusEvent              = EventView<EventType::FanStatus>;
using FanSampleEvent              = EventView<EventType::FanSample>;
using FanCommandEvent             = EventView<EventType::FanCommand>;
using HeatpadStatusEvent          = EventView<EventType::HeatpadStatus>;
using HeatpadSampleEvent          = EventView<EventType::HeatpadSample>;
using HeatpadCommandEvent         = EventView<EventType::HeatpadCommand>;
using TemperatureStatusEvent      = EventView<EventType::TemperatureStatus>;
using TemperatureSampleEvent      = EventView<EventType::TemperatureSample>;
using ButtonStateChangedEvent     = EventView<EventType::ButtonStateChanged>;
using ButtonRepeatEvent           = EventView<EventType::ButtonRepeat>;
using EncoderStepEvent            = EventView<EventType::EncoderStep>;
using ActiveBehaviourChangedEvent = EventView<EventType::ActiveBehaviourChanged>;
using RequestChangeBehaviourEvent = EventView<EventType::RequestChangeBehaviour>;
using ActiveScreenChangedEvent    = EventView<EventType::ActiveScreenChanged>;
using RequestChangeScreenEvent    = EventView<EventType::RequestChangeScreen>;
using RequestUpdateScreensEvent   = EventView<EventType::RequestUpdateScreens>;

// event payload aliases
using HeartbeatPayload              = EventPayload<EventType::Heartbeat>;
using FermentationStatusPayload     = EventPayload<EventType::FermentationStatus>;
using BacklightCommandPayload       = EventPayload<EventType::BacklightCommand>;
using FanStatusPayload              = EventPayload<EventType::FanStatus>;
using FanSamplePayload              = EventPayload<EventType::FanSample>;
using FanCommandPayload             = EventPayload<EventType::FanCommand>;
using HeatpadStatusPayload          = EventPayload<EventType::HeatpadStatus>;
using HeatpadSamplePayload          = EventPayload<EventType::HeatpadSample>;
using HeatpadCommandPayload         = EventPayload<EventType::HeatpadCommand>;
using TemperatureStatusPayload      = EventPayload<EventType::TemperatureStatus>;
using TemperatureSamplePayload      = EventPayload<EventType::TemperatureSample>;
using ButtonStateChangedPayload     = EventPayload<EventType::ButtonStateChanged>;
using ButtonRepeatPayload           = EventPayload<EventType::ButtonRepeat>;
using EncoderStepPayload            = EventPayload<EventType::EncoderStep>;
using ActiveBehaviourChangedPayload = EventPayload<EventType::ActiveBehaviourChanged>;
using RequestChangeBehaviourPayload = EventPayload<EventType::RequestChangeBehaviour>;
using ActiveScreenChangedPayload    = EventPayload<EventType::ActiveScreenChanged>;
using RequestChangeScreenPayload    = EventPayload<EventType::RequestChangeScreen>;
using RequestUpdateScreensPayload   = EventPayload<EventType::RequestUpdateScreens>;

} // namespace Garbox