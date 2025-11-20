#include "EventForwarder.h"

namespace Garbox {

EventForwarder::EventForwarder(){
    // nothing to do
}

void EventForwarder::setHandler(Handler handler){
    mHandler = handler;
}

void EventForwarder::forward(const EventHeader* header){
    if(!mHandler){
        TriggerDebug("EventForwarder", "no handler set");
        return;
    }
    mHandler(header);
}

} // namespace Garbox
