// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "InputControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

InputControllerAbs::InputControllerAbs() : ControllerAbs(ComponentId::InputController, ControllerId::Input){
    // nothing to do
}

ButtonStateChangedEvent InputControllerAbs::makeButtonStateChangedEvent(){
    return ComponentAbs::makeEvent<EventType::ButtonStateChanged>();
}

ButtonRepeatEvent InputControllerAbs::makeButtonRepeatEvent(){
    return ComponentAbs::makeEvent<EventType::ButtonRepeat>();
}

EncoderStepEvent InputControllerAbs::makeEncoderStepEvent(){
    return ComponentAbs::makeEvent<EventType::EncoderStep>();
}

void InputControllerAbs::sendEvent(const ButtonStateChangedEvent& event){
    sendEventToHost(event.header());
}

void InputControllerAbs::sendEvent(const ButtonRepeatEvent& event){
    sendEventToHost(event.header());
}

void InputControllerAbs::sendEvent(const EncoderStepEvent& event){
    sendEventToHost(event.header());
}

} // namespace Garbox