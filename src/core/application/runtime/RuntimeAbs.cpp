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
    mControllersSpan(nullptr, 0),
    mBehavioursSpan(nullptr, 0){
    // constructor body
}

void RuntimeAbs::init(){

    // derived class must register controllers and behaviours
    onRegisterControllers();
    onRegisterBehaviours();

    // set all controller hosts
    for(ControllerAbs* controller : mControllersSpan){
        controller->setControllerHost(*this);
    }

    // set all behaviours hosts
    for(BehaviourAbs* behaviour : mBehavioursSpan){
        behaviour->setBehaviourHost(*this);
    }

    // init all controllers
    for(ControllerAbs* controller : mControllersSpan){
        controller->init();
    }

    // init all behaviours
    for(BehaviourAbs* behaviour : mBehavioursSpan){
        behaviour->init();
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

void RuntimeAbs::beginTickSequence(){
    incrementTickCount();
    applyQueuedBehaviour();
}

void RuntimeAbs::setQueuedBehaviour(BehaviourAbs* behaviour){
    mQueuedBehaviour = behaviour;
}

void RuntimeAbs::applyQueuedBehaviour() {
    if(mQueuedBehaviour){
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
}

void RuntimeAbs::requestChangeBehaviour(BehaviourId id){
    BehaviourAbs* behaviour = resolveBehaviour(id);
    AssertExit(behaviour != nullptr, "RuntimeAbs", "failed to resolve behaviour", static_cast<uint32_t>(id));
    setQueuedBehaviour(behaviour);
}

BehaviourAbs* RuntimeAbs::getActiveBehaviour() const {
    return mActiveBehaviour;
}

void RuntimeAbs::registerController(ControllerAbs* controller){
    const size_t maxIndex = MaxControllersCount - 1;
    const size_t nextIndex = mControllersSpan.size();
    AssertExit(nextIndex <= maxIndex, "RuntimeAbs", "max controllers count exceeded");
    mControllersRawArray[nextIndex] = controller;
    mControllersSpan = Span<ControllerAbs*>(mControllersRawArray, mControllersSpan.size()+1);
}

Span<ControllerAbs*> RuntimeAbs::getControllers(){
    return mControllersSpan;
}

void RuntimeAbs::registerBehaviour(BehaviourAbs* behaviour){
    const size_t maxIndex = MaxBehavioursCount - 1;
    const size_t nextIndex = mBehavioursSpan.size();
    AssertExit(nextIndex <= maxIndex, "RuntimeAbs", "max behaviours count exceeded");
    mBehavioursRawArray[nextIndex] = behaviour;
    mBehavioursSpan = Span<BehaviourAbs*>(mBehavioursRawArray, mBehavioursSpan.size()+1);
}

Span<BehaviourAbs*> RuntimeAbs::getBehaviours(){
    return mBehavioursSpan;
}

void RuntimeAbs::publishEvent(const EventHeader* header){
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

void RuntimeAbs::incrementTickCount(){
    mContext.tickCount++;
}

const RuntimeContext& RuntimeAbs::getContext() const {
    return mContext;
}

} // namespace