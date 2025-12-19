#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"

namespace Garbox {

class Runtime;

class InputControllerAbs : public ControllerAbs {
public:

    // component constructor
    InputControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() {};

protected:

    // make typed events 
    ButtonEvent makeButtonEvent();
    ButtonRepeatEvent makeButtonRepeatEvent();
    EncoderStepEvent makeEncoderStepEvent();

    // send typed events
    void sendEvent(const ButtonEvent& event);
    void sendEvent(const ButtonRepeatEvent& event);
    void sendEvent(const EncoderStepEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox