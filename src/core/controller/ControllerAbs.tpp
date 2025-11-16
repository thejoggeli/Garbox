#include "ControllerAbs.h"

#include "assert/Assert.h"
#include "core/event/EventSystem.h"

namespace Garbox {

ControllerAbs::ControllerAbs(){
    // constructor body
}

ControllerAbs::~ControllerAbs(){
    TriggerExit("ControllerAbs", "controllers must not be destroyed");
}

void ControllerAbs::setup(EventSystem& eventSystem){
    mEventSystem = &eventSystem;
}

void ControllerAbs::tick(){
    onTick();
}

template<typename EventData>
void ControllerAbs::sendEvent(const EventData& event){
    mEventSystem->send<EventData>(data);
}

} // namespace