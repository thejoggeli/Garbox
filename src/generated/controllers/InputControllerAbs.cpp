// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "InputControllerAbs.h"

namespace Garbox {

InputControllerAbs::InputControllerAbs() : ControllerAbs(ComponentId::InputController, ControllerId::Input){
    // nothing to do
}

ButtonEvent InputControllerAbs::makeButtonEvent(){
    return ComponentAbs::makeEvent<EventType::Button>();
}

ButtonRepeatEvent InputControllerAbs::makeButtonRepeatEvent(){
    return ComponentAbs::makeEvent<EventType::ButtonRepeat>();
}

EncoderStepEvent InputControllerAbs::makeEncoderStepEvent(){
    return ComponentAbs::makeEvent<EventType::EncoderStep>();
}

void InputControllerAbs::sendEvent(const ButtonEvent& event){
    sendEventToHost(event.header());
}

void InputControllerAbs::sendEvent(const ButtonRepeatEvent& event){
    sendEventToHost(event.header());
}

void InputControllerAbs::sendEvent(const EncoderStepEvent& event){
    sendEventToHost(event.header());
}

} // namespace Garbox