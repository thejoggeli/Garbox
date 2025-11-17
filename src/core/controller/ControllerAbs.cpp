#include "ControllerAbs.h"

#include "assert/Assert.h"
#include "core/event/EventForwarder.h"

namespace Garbox {

ControllerAbs::ControllerAbs(ControllerId controllerId) : mControllerId(controllerId) {
    // constructor body
}

ControllerAbs::~ControllerAbs(){
    TriggerExit("ControllerAbs", "controllers must not be destroyed");
}

void ControllerAbs::setup(EventFactory& factory, EventForwarder& forwarder){;
    mEventFactory = &factory;
    mEventForwarder = &forwarder;
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

void ControllerAbs::sendEvent(Event* event){
    event->sender = mControllerId;
    mEventForwarder->forward(event);
}

} // namespace