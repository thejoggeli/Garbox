// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "InputControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

InputControllerAbs::InputControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::InputController, context){
    // nothing to do
}

EventWrite<EventPayload::ButtonStateChanged> InputControllerAbs::makeButtonStateChangedEvent(){
    return makeEvent<EventPayload::ButtonStateChanged>();
}

EventWrite<EventPayload::ButtonRepeat> InputControllerAbs::makeButtonRepeatEvent(){
    return makeEvent<EventPayload::ButtonRepeat>();
}

EventWrite<EventPayload::EncoderStep> InputControllerAbs::makeEncoderStepEvent(){
    return makeEvent<EventPayload::EncoderStep>();
}

void InputControllerAbs::sendEvent(const EventWrite<EventPayload::ButtonStateChanged>& event){
    sendEvent(event.header);
}

void InputControllerAbs::sendEvent(const EventWrite<EventPayload::ButtonRepeat>& event){
    sendEvent(event.header);
}

void InputControllerAbs::sendEvent(const EventWrite<EventPayload::EncoderStep>& event){
    sendEvent(event.header);
}

} // namespace Garbox