#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include <cstdint>
#include <cstring>
#include "shared/types/EventType.h"
#include "app/engine/HeaterEngineState.h"
#include "modules/parts/button/ButtonState.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

// generated payloads
struct HeartbeatPayload {
    // no fields
};

struct FermentationStatusPayload {
    HeaterEngineState heaterEngineState;
    float targetTemperature;
    float measuredTemperature;
    float measuredHumidity;
};

struct BacklightCommandPayload {
    float brightness;
};

struct FanStatusPayload {
    FanState state;
    float targetSpeed;
    bool rpmControlEnabled;
};

struct FanSamplePayload {
    float measuredRpm;
};

struct FanCommandPayload {
    bool enabled;
    float targetSpeed;
    bool enableRpmControl;
};

struct HeatpadStatusPayload {
    HeatpadState state;
    float currentDutyCycle;
    uint32_t currentPeriodMicros;
    float nextDutyCycle;
    uint32_t nextPeriodMicros;
    float pwmProgressMicros;
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
    uint32_t holdTimeMicros;
};

struct EncoderStepPayload {
    int32_t steps;
};

struct ActiveBehaviourChangedPayload {
    BehaviourId oldBehaviour;
    BehaviourId newBehaviour;
};

struct RequestChangeBehaviourPayload {
    BehaviourId behaviour;
};

struct ActiveScreenChangedPayload {
    ScreenId oldScreen;
    ScreenId newScreen;
};

struct RequestChangeScreenPayload {
    ScreenId screen;
};

struct RequestUpdateScreensPayload {
    // no fields
};

// generated mappings
template<EventType>
struct ResolveEventPayload; // primary template (intentionally undefined)

template<>
struct ResolveEventPayload<EventType::Heartbeat> {
    using type = HeartbeatPayload;
};

template<>
struct ResolveEventPayload<EventType::FermentationStatus> {
    using type = FermentationStatusPayload;
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

template<>
struct ResolveEventPayload<EventType::ActiveBehaviourChanged> {
    using type = ActiveBehaviourChangedPayload;
};

template<>
struct ResolveEventPayload<EventType::RequestChangeBehaviour> {
    using type = RequestChangeBehaviourPayload;
};

template<>
struct ResolveEventPayload<EventType::ActiveScreenChanged> {
    using type = ActiveScreenChangedPayload;
};

template<>
struct ResolveEventPayload<EventType::RequestChangeScreen> {
    using type = RequestChangeScreenPayload;
};

template<>
struct ResolveEventPayload<EventType::RequestUpdateScreens> {
    using type = RequestUpdateScreensPayload;
};

// resolve EventType to EventPayload
template<EventType E>
using EventPayload = typename ResolveEventPayload<E>::type;

} // namespace 