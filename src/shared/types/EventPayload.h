#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>
#include <cstring>
#include "shared/types/EventType.h"
#include "shared/types/BehaviourId.h"
#include "modules/parts/button/ButtonState.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

// generated payloads
struct ActiveBehaviourChangedPayload {
    BehaviourId oldBehaviour;
    BehaviourId newBehaviour;
};

struct HeartbeatPayload {
    // no fields
};

struct BacklightCommandPayload {
    float brightness;
};

struct FanStatusPayload {
    FanState state;
    float targetSpeed;
};

struct FanSamplePayload {
    float measuredRpm;
};

struct FanCommandPayload {
    bool enabled;
    float targetSpeed;
};

struct HeatpadStatusPayload {
    HeatpadState state;
    float dutyCycle;
    uint32_t periodMicros;
};

struct HeatpadSamplePayload {
    HeatpadState state;
    float measuredVoltage;
    float measuredCurrent;
};

struct HeatpadCommandPayload {
    bool enabled;
    float dutyCycle;
    uint32_t periodMicros;
};

struct TemperatureStatusPayload {
    bool driverEnabled;
    bool powerEnabled;
    bool resetting;
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
};

struct EncoderStepPayload {
    int32_t steps;
};


// generated mappings
template<EventType>
struct ResolveEventPayload; // primary template (intentionally undefined)

template<>
struct ResolveEventPayload<EventType::ActiveBehaviourChanged> {
    using type = ActiveBehaviourChangedPayload;
};

template<>
struct ResolveEventPayload<EventType::Heartbeat> {
    using type = HeartbeatPayload;
};

template<>
struct ResolveEventPayload<EventType::BacklightCommand> {
    using type = BacklightCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::FanStatus> {
    using type = FanStatusPayload;
};

template<>
struct ResolveEventPayload<EventType::FanSample> {
    using type = FanSamplePayload;
};

template<>
struct ResolveEventPayload<EventType::FanCommand> {
    using type = FanCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::HeatpadStatus> {
    using type = HeatpadStatusPayload;
};

template<>
struct ResolveEventPayload<EventType::HeatpadSample> {
    using type = HeatpadSamplePayload;
};

template<>
struct ResolveEventPayload<EventType::HeatpadCommand> {
    using type = HeatpadCommandPayload;
};

template<>
struct ResolveEventPayload<EventType::TemperatureStatus> {
    using type = TemperatureStatusPayload;
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

// resolve EventType to EventPayload
template<EventType E>
using EventPayload = typename ResolveEventPayload<E>::type;

} // namespace 