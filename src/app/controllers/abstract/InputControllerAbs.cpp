// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "InputControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

InputControllerAbs::InputControllerAbs() : ControllerAbs(ComponentId::InputController, ControllerId::Input){
    // nothing to do
}

void InputControllerAbs::receiveTick(TickPhase phase){
    switch(phase){
        case TickPhase::Input: onInputTick(); break;
        default: TriggerDebug("InputControllerAbs", "received unhandled tick");
    };
};

void InputControllerAbs::receiveEvent(const EventHeader* header){
    // no events configured
};

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
    publishEvent(event.header());
}

void InputControllerAbs::sendEvent(const ButtonRepeatEvent& event){
    publishEvent(event.header());
}

void InputControllerAbs::sendEvent(const EncoderStepEvent& event){
    publishEvent(event.header());
}
 
} // namespace Garbox