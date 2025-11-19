#include "EventForwarder.h"

namespace Garbox {

EventForwarder::EventForwarder(){
    // nothing to do
}

void EventForwarder::setHandler(const Handler& handler){
    mHandler = handler;
}

void EventForwarder::forward(const Event* event){
    if(!mHandler){
        TriggerDebug("EventForwarder", "no handler set");
        return;
    }
    mHandler(event);
}

} // namespace Garbox
