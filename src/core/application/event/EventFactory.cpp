#include "EventFactory.h"

namespace Garbox {

EventFactory::EventFactory(size_t poolSizeBytes) : mPool(poolSizeBytes) {
    // nothing to do
}

EventFactory::~EventFactory(){ 
    TriggerExit("EventFactory", "heap deconstructor");
}

void EventFactory::releaseDataPool(){
    mPool.releaseAll();
}

} // namespace
