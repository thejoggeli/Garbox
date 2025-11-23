#pragma once

// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class InputControllerAbs : public ControllerAbs {
public:
    
    InputControllerAbs();

    // tick handlers
    virtual void onInputTick() = 0;

protected:

    // make typed events
    ButtonStateChangedEvent makeButtonStateChangedEvent();
    ButtonRepeatEvent makeButtonRepeatEvent();
    EncoderStepEvent makeEncoderStepEvent();

    // send typed events
    void sendEvent(const ButtonStateChangedEvent& event);
    void sendEvent(const ButtonRepeatEvent& event);
    void sendEvent(const EncoderStepEvent& event);

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox