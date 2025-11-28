#include "RuntimeAbs.h"

#define GarboxDebugRuntimeAbs 1

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/assert/Assert.h"

#if GarboxDebugRuntimeAbs
#include "core/log/Log.h"
#include "core/time/Time.h"
#endif

namespace Garbox {

RuntimeAbs::RuntimeAbs(const Config& config):
    // init members
    mEventFactory(config.eventPoolSizeBytes),
    mEventQueue(config.eventQueueLength),
    mControllers(config.maxControllers),
    mControllersSpan(nullptr, 0),
    mBehaviours(config.maxBehaviours),
    mBehavioursSpan(nullptr, 0){
    // constructor body
}

void RuntimeAbs::init(){

    // derived class must register controllers, behaviours and ticks
    onRegister();

    // init all controllers
    for(ControllerAbs* controller : mControllersSpan){
        controller->init(*this);
    }

    // init all behaviours
    for(BehaviourAbs* behaviour : mBehavioursSpan){
        behaviour->init(*this);
    }

    // init derived class
    onInit();
}

void RuntimeAbs::start(){

    AssertExit(mBehavioursSpan.size() > 0, "RuntimeAbs", "no behaviours registered");
    AssertExit(mControllersSpan.size() > 0, "RuntimeAbs", "no controllers registered");

    // start all controllers
    for(ControllerAbs* controller : mControllersSpan){
        controller->start();
    }

    // start all behaviours
    for(BehaviourAbs* behaviour : mBehavioursSpan){
        behaviour->start();
    }

    // start derived class
    onStart();
}

void RuntimeAbs::run(){
    onRun();
}

void RuntimeAbs::setQueuedBehaviour(BehaviourAbs* behaviour){
    mQueuedBehaviour = behaviour;
}

void RuntimeAbs::applyQueuedBehaviour() {

    if(!mQueuedBehaviour){
        return;
    }

    if(mQueuedBehaviour == mActiveBehaviour){
        TriggerDebug("RuntimeAbs", "queued behaviour is already active");
        return;
    }

    // create event
    EventView event = mEventFactory.make<EventType::ActiveBehaviourChanged>(mComponentDescriptor);
    event->newBehaviour = mQueuedBehaviour->getBehaviourId();

    // set queued behaviour active
    if(mActiveBehaviour){
        mActiveBehaviour->setActive(false);
        event->oldBehaviour = mActiveBehaviour->getBehaviourId();
    }
    else {
        event->oldBehaviour = BehaviourId::Null;
    }
    mActiveBehaviour = mQueuedBehaviour;
    mQueuedBehaviour = nullptr;
    mActiveBehaviour->setActive(true);

    // send event
    publishEvent(event.header());   
}

void RuntimeAbs::registerController(ControllerAbs* controller){
    AssertExit(!mControllers.full(), "RuntimeAbs", "max controllers count exceeded");
    mControllers.push(controller);
    mControllersSpan = Span<ControllerAbs*>(&mControllers[0], mControllers.size());
}

Span<ControllerAbs*> RuntimeAbs::getControllers(){
    return mControllersSpan;
}

void RuntimeAbs::registerBehaviour(BehaviourAbs* behaviour){
    AssertExit(!mBehaviours.full(), "RuntimeAbs", "max behaviours count exceeded");
    mBehaviours.push(behaviour);
    mBehavioursSpan = Span<BehaviourAbs*>(&mBehaviours[0], mBehaviours.size());
}

Span<BehaviourAbs*> RuntimeAbs::getBehaviours(){
    return mBehavioursSpan;
}

void RuntimeAbs::publishEvent(const EventHeader* header){

    // handle change behaviour request
    if(header->type == EventType::RequestChangeBehaviour){
        RequestChangeBehaviourEvent event(header);
        setQueuedBehaviour(resolveBehaviour(event->behaviour));
    }

    // send event to router event
    mContext.eventCount++;
    if(header->bypassQueue){
        onRouteEvent(header);
    }
    else if(!mEventQueue.pushBack(header)){
        TriggerExit("RuntimeAbs", "event queue is full");
    }
}

EventFactory& RuntimeAbs::getEventFactory(){
    return mEventFactory;
}

void RuntimeAbs::clearEventQueue(){
    mEventQueue.releaseAll();
    mEventFactory.releaseDataPool();
}

void RuntimeAbs::dispatchEvents(){
    const EventHeader* header;
    while(mEventQueue.releaseFront(header)){
        if(header == nullptr){
            TriggerDebug("RuntimeAbs", "event header is nullptr");
            continue;
        }
        onRouteEvent(header);
    }
    mEventFactory.releaseDataPool();
}

const RuntimeContext& RuntimeAbs::getContext() const {
    return mContext;
}

} // namespace