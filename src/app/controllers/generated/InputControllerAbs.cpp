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
    return ControllerAbs::makeEvent<EventType::ButtonStateChanged>();
}

ButtonRepeatEvent InputControllerAbs::makeButtonRepeatEvent(){
    return ControllerAbs::makeEvent<EventType::ButtonRepeat>();
}

EncoderStepEvent InputControllerAbs::makeEncoderStepEvent(){
    return ControllerAbs::makeEvent<EventType::EncoderStep>();
}

void InputControllerAbs::sendEvent(const ButtonStateChangedEvent& event){
    publishEvent(event.header());
}

void InputControllerAbs::sendEvent(const ButtonRepeatEvent& event){
    publishEvent(event.header());
}

void InputControllerAbs::sendEvent(const EncoderStepEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox