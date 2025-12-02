#include "EventLogScreen.h"

#define GarboxEventLogScreenToConsole 0
#include "core/log/Log.h"

namespace Garbox {

EventLogScreen::EventLogScreen() : EventLogScreenAbs(){
    // nothing to do
}

void EventLogScreen::onInit(){
    // nothing to do
}

void EventLogScreen::onStart(){
    // nothing to do
}

void EventLogScreen::onBecomeEnabled(){
    // nothing to do
}

void EventLogScreen::onBecomeDisabled(){
    // nothing to do
}

void EventLogScreen::onUpdateScreen(){
    // nothing to do   
}

void EventLogScreen::onEvent(const EventHeader* header){
#if GarboxEventLogScreenToConsole
    LogDebug("Event", "[%u] %s: %s", header->id, ComponentIdToString(header->sender.id), EventTypeToString(header->type));
#endif
}

} // namespace Garbox