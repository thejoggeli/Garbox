#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/behaviour/BehaviourAbs.h"
#include "generated/components/behaviours/fermentation/FermentationBehaviourStates.h"

namespace Garbox {

class Runtime;

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    FermentationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) = 0;
    virtual void onButtonEvent(const ButtonEvent& event) = 0;
    virtual void onButtonRepeatEvent(const ButtonRepeatEvent& event) = 0;
    virtual void onEncoderStepEvent(const EncoderStepEvent& event) = 0;
    virtual void onRequestFermentationModeEvent(const RequestFermentationModeEvent& event) = 0;
    virtual void onRequestTargetTemperatureEvent(const RequestTargetTemperatureEvent& event) = 0;
    virtual void onRequestFanModeEvent(const RequestFanModeEvent& event) = 0;

    // state change handlers (to be implmeneted by user)
    virtual void onFanSampleStateChanged(const FanSampleState& state) = 0;
    virtual void onFanStatusStateChanged(const FanStatusState& state) = 0;
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) = 0;
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) = 0;

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();
    DisplayCommandEvent makeDisplayCommandEvent();
    HeatpadCommandEvent makeHeatpadCommandEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);
    void sendEvent(const DisplayCommandEvent& event);
    void sendEvent(const HeatpadCommandEvent& event);

    FermentationBehaviourStates& states();

private:

    std::optional<FermentationBehaviourStates> mStates;

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox