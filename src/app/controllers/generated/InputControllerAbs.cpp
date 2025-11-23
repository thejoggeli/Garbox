// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "InputControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

InputControllerAbs::InputControllerAbs():
    // init memberes
    ControllerAbs(ComponentId::InputController, ControllerId::Input){
    // nothing to do
}

ButtonStateChangedEvent InputControllerAbs::makeButtonStateChangedEvent(){
    return makeEvent<EventType::ButtonStateChanged>();
}

ButtonRepeatEvent InputControllerAbs::makeButtonRepeatEvent(){
    return makeEvent<EventType::ButtonRepeat>();
}

EncoderStepEvent InputControllerAbs::makeEncoderStepEvent(){
    return makeEvent<EventType::EncoderStep>();
}

void InputControllerAbs::sendEvent(const ButtonStateChangedEvent& event){
    sendEvent(event);
}

void InputControllerAbs::sendEvent(const ButtonRepeatEvent& event){
    sendEvent(event);
}

void InputControllerAbs::sendEvent(const EncoderStepEvent& event){
    sendEvent(event);
}

} // namespace Garbox