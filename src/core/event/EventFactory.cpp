#include "EventFactory.h"

namespace Garbox {

EventFactory::EventFactory(){ 
    AssertExit(!mInitialized, "EventFactory", "heap deconstructor");
}

EventFactory::~EventFactory(){ 
    AssertExit(!mInitialized, "EventFactory", "heap deconstructor");
}

void EventFactory::init(size_t poolSizeBytes){
    AssertExit(!mInitialized, "EventFactory", "already initialized");
    mPool.init(poolSizeBytes);
    mInitialized = true;
}

void EventFactory::clearDataPool(){
    mPool.clear();
}

} // namespace