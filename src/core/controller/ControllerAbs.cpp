#include "ControllerAbs.h"

#include "assert/Assert.h"

namespace Garbox {

ControllerAbs::ControllerAbs(){
    // constructor body
}

ControllerAbs::~ControllerAbs(){
    TriggerExit("ControllerAbs", "controllers must not be destroyed");
}

void ControllerAbs::setup(ControllerId controllerId, EventFactory& eventFactory){
    mControllerId = controllerId;
    mEventFactory = &eventFactory;
}

void ControllerAbs::init(){
    onInit();
}

void ControllerAbs::start(){
    onStart();
}

void ControllerAbs::tick(){
    onTick();
}

ControllerId ControllerAbs::getControllerId(){
    return mControllerId;
}

EventFactory& ControllerAbs::getEventFactory(){
    return *mEventFactory;
}

void ControllerAbs::sendEvent(Event* header){
    if(header == nullptr){
        TriggerDebug("ControllerAbs", "event header is nullptr", static_cast<uint32_t>(mControllerId));
        return;
    }
    header->sender = mControllerId;
    // TODO do something with header
}

} // namespace