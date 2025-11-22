#include "RuntimeAbs.h"

#define GarboxDebugRuntimeAbs 0

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/assert/Assert.h"
#include "core/time/Time.h"

#if GarboxDebugRuntimeAbs
#include "core/log/Log.h"
#endif

namespace Garbox {

RuntimeAbs::RuntimeAbs():
    // init members
    mControllersSpan(nullptr, 0){
    // constructor body
}

void RuntimeAbs::init(const Config& config){

    // init event factory
    mEventFactory.init(config.eventPoolSizeBytes);

    // init event queue
    mEventQueue.init(config.eventQueueLength);

    // derived class must register controllers and behaviours
    onRegisterControllers();
    onRegisterBehaviours();

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
        EventWrite event = mEventFactory.make<EventPayload::ActiveBehaviourChanged>(mComponentDescriptor);
        event.payload->newBehaviour = mQueuedBehaviour->getBehaviourId();

        // set queued behaviour active
        if(mActiveBehaviour){
            mActiveBehaviour->setActive(false);
            event.payload->oldBehaviour = mActiveBehaviour->getBehaviourId();
        }
        else {
            event.payload->oldBehaviour = BehaviourId::Null;
        }
        mActiveBehaviour = mQueuedBehaviour;
        mQueuedBehaviour = nullptr;
        mActiveBehaviour->setActive(true);

        // send event
        publishEvent(event.header);
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
    else if(!mEventQueue.push(header)){
        TriggerExit("RuntimeAbs", "event queue is full");
    }
}

EventFactory& RuntimeAbs::getEventFactory(){
    return mEventFactory;
}

void RuntimeAbs::clearEventQueue(){
    mEventQueue.clear();
    mEventFactory.clearDataPool();
}

void RuntimeAbs::dispatchEvents(){
    const EventHeader* header;
    while(mEventQueue.pop(header)){
        if(header == nullptr){
            TriggerDebug("RuntimeAbs", "event header is nullptr");
            continue;
        }
        onRouteEvent(header);
    }
    mEventFactory.clearDataPool();
}

void RuntimeAbs::incrementTickCount(){
    mContext.tickCount++;
}

const RuntimeContext& RuntimeAbs::getContext() const {
    return mContext;
}

} // namespace