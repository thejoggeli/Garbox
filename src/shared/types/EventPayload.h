#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>
#include <cstring>
#include "shared/types/EventType.h"
    

namespace Garbox {

// generated payloads
struct HeartbeatPayload {
    // no fields
};

struct DisplayCommandPayload {
    float brightness;
};

struct FanCommandPayload {
    bool enabled;
    float targetSpeed;
    bool enableRpmControl;
};

struct HeatpadCommandPayload {
    bool enabled;
    float dutyCycle;
    uint32_t periodMicros;
};

struct TemperatureSamplePayload {
    float temperatureCelcius;
    float humidityRelative;
};

struct ButtonStateChangedPayload {
    ButtonState oldState;
    ButtonState newState;
};

struct ButtonRepeatPayload {
    uint32_t count;
    uint32_t holdTimeMicros;
};

struct EncoderStepPayload {
    int32_t steps;
};

struct ActiveBehaviourChangedPayload {
    BehaviourId oldBehaviour;
    BehaviourId newBehaviour;
};

struct ActiveScreenChangedPayload {
    ScreenId oldScreen;
    ScreenId newScreen;
};

// generated mappings
template<EventType>
struct ResolveEventPayload; // primary template (intentionally undefined)

template<>
struct ResolveEventPayload<EventType::Heartbeat> {
    using type = HeartbeatPayload;
};

template<>
struct ResolveEventPayload<EventType::DisplayCommand> {
    using type = DisplayCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::FanCommand> {
    using type = FanCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::HeatpadCommand> {
    using type = HeatpadCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::TemperatureSample> {
    using type = TemperatureSamplePayload;
};

template<>
struct ResolveEventPayload<EventType::ButtonStateChanged> {
    using type = ButtonStateChangedPayload;
};

template<>
struct ResolveEventPayload<EventType::ButtonRepeat> {
    using type = ButtonRepeatPayload;
};

template<>
struct ResolveEventPayload<EventType::EncoderStep> {
    using type = EncoderStepPayload;
};

template<>
struct ResolveEventPayload<EventType::ActiveBehaviourChanged> {
    using type = ActiveBehaviourChangedPayload;
};

template<>
struct ResolveEventPayload<EventType::ActiveScreenChanged> {
    using type = ActiveScreenChangedPayload;
};

// resolve EventType to EventPayload
template<EventType E>
using EventPayload = typename ResolveEventPayload<E>::type;

} // namespace 