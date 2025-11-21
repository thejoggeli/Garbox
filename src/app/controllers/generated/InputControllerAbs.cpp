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

InputControllerAbs::ButtonStateChangedWrite InputControllerAbs::makeButtonStateChangedEvent(){
    return makeEvent<EventPayload::ButtonStateChanged>();
}

InputControllerAbs::ButtonRepeatWrite InputControllerAbs::makeButtonRepeatEvent(){
    return makeEvent<EventPayload::ButtonRepeat>();
}

InputControllerAbs::EncoderStepWrite InputControllerAbs::makeEncoderStepEvent(){
    return makeEvent<EventPayload::EncoderStep>();
}

void InputControllerAbs::sendEvent(const ButtonStateChangedWrite& event){
    sendEvent(event.header);
}

void InputControllerAbs::sendEvent(const ButtonRepeatWrite& event){
    sendEvent(event.header);
}

void InputControllerAbs::sendEvent(const EncoderStepWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox