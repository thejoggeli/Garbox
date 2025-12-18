#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/states/types/FanStatusState.h"
#include "app/states/types/FanSampleState.h"

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class GarboxRuntime;

class FanControllerAbs : public ControllerAbs {
public:

    // component constructor
    FanControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onFanCommandEvent(const FanCommandEvent& event) = 0;

protected:

    // make typed events 
    FanStatusEvent makeFanStatusEvent();
    FanSampleEvent makeFanSampleEvent();

    // send typed events
    void sendEvent(const FanStatusEvent& event);
    void sendEvent(const FanSampleEvent& event);

    // get writable states
    FanStatusState& writeFanStatusState();
    FanSampleState& writeFanSampleState();

private:

    // writable state pointers
    FanStatusState* mFanStatusState = nullptr;
    FanSampleState* mFanSampleState = nullptr;

    // dependency inject writable states
    void injectFanStatusState(FanStatusState* state);
    void injectFanSampleState(FanSampleState* state);

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class GarboxRuntime;
};

} // namespace Garbox