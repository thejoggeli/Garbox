#include "RuntimeAbs.h"

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/application/controller/ControllerIfc.h"
#include "core/assert/Assert.h"
#include "core/time/Time.h"

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
    for(ControllerIfc* controller : mControllersSpan){
        controller->init(*this);
    }

    // init all behaviours
    for(BehaviourIfc* behaviour : mBehavioursSpan){
        behaviour->init(*this);
    }

    // init derived class
    onInit();
}

void RuntimeAbs::start(){

    AssertExit(mBehavioursSpan.size() > 0, "RuntimeAbs", "no behaviours registered");
    AssertExit(mControllersSpan.size() > 0, "RuntimeAbs", "no controllers registered");

    // start all controllers
    for(ControllerIfc* controller : mControllersSpan){
        controller->start();
    }

    // start all behaviours
    for(BehaviourIfc* behaviour : mBehavioursSpan){
        behaviour->start();
    }

    // start derived class
    onStart();
}

void RuntimeAbs::beginTickSequence(){
    incrementTickCount();
    applyQueuedBehaviour();
}

void RuntimeAbs::setQueuedBehaviour(BehaviourIfc* behaviour){
    mQueuedBehaviour = behaviour;
}

void RuntimeAbs::applyQueuedBehaviour() {
    if(mQueuedBehaviour){
        mActiveBehaviour = mQueuedBehaviour;
        mQueuedBehaviour = nullptr;
    }
}

void RuntimeAbs::requestChangeBehaviour(ComponentId id){
    // TODO implement
    TriggerExit("RuntimeAbs", "requestChangeBehaviour not implemented");
}

BehaviourIfc* RuntimeAbs::getActiveBehaviour() const {
    return mActiveBehaviour;
}

void RuntimeAbs::registerController(ControllerIfc* controller){
    const size_t maxIndex = MaxControllersCount - 1;
    const size_t nextIndex = mControllersSpan.size();
    AssertExit(nextIndex <= maxIndex, "RuntimeAbs", "max controllers count exceeded");
    mControllersRawArray[nextIndex] = controller;
    mControllersSpan = Span<ControllerIfc*>(mControllersRawArray, mControllersSpan.size()+1);
}

Span<ControllerIfc*> RuntimeAbs::getControllers(){
    return mControllersSpan;
}

void RuntimeAbs::registerBehaviour(BehaviourIfc* behaviour){
    const size_t maxIndex = MaxBehavioursCount - 1;
    const size_t nextIndex = mBehavioursSpan.size();
    AssertExit(nextIndex <= maxIndex, "RuntimeAbs", "max behaviours count exceeded");
    mBehavioursRawArray[nextIndex] = behaviour;
    mBehavioursSpan = Span<BehaviourIfc*>(mBehavioursRawArray, mBehavioursSpan.size()+1);
}

Span<BehaviourIfc*> RuntimeAbs::getBehaviours(){
    return mBehavioursSpan;
}

void RuntimeAbs::publishEvent(const EventHeader* header){
    if(!mEventQueue.push(header)){
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