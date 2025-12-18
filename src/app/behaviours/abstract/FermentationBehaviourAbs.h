#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/FermentationStatusState.h"

#include "core/application/behaviour/BehaviourAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class FermentationBehaviourAbs : public BehaviourAbs {
public:

    // component constructor
    FermentationBehaviourAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLogicTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeartbeatEvent(const HeartbeatEvent& event) = 0;
    virtual void onButtonStateChangedEvent(const ButtonStateChangedEvent& event) = 0;
    virtual void onButtonRepeatEvent(const ButtonRepeatEvent& event) = 0;
    virtual void onEncoderStepEvent(const EncoderStepEvent& event) = 0;

protected:

    // make typed events 
    FanCommandEvent makeFanCommandEvent();
    DisplayCommandEvent makeDisplayCommandEvent();
    HeatpadCommandEvent makeHeatpadCommandEvent();
    FermentationStatusEvent makeFermentationStatusEvent();

    // send typed events
    void sendEvent(const FanCommandEvent& event);
    void sendEvent(const DisplayCommandEvent& event);
    void sendEvent(const HeatpadCommandEvent& event);
    void sendEvent(const FermentationStatusEvent& event);

    // state type accessors for getters and setters
    struct FermentationStatusType {};
    static constexpr FermentationStatusType FermentationStatus {};

    // get writable states
    FermentationStatusState& writeState(FermentationStatusType type);

private:

    // writable state pointers
    FermentationStatusState* mFermentationStatusState = nullptr;

    // dependency inject writable states
    void injectWritableState(FermentationStatusState* state);

    // hide event methods
    using BehaviourAbs::makeEvent;
    using BehaviourAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox