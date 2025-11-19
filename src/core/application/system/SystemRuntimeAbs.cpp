#include "SystemRuntimeAbs.h"

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/assert/Assert.h"
#include "core/controller/ControllerIfc.h"

namespace Garbox {

SystemRuntimeAbs::SystemRuntimeAbs():
    // init members
    mControllersSpan(nullptr, 0){
    // constructor body
}

void SystemRuntimeAbs::init(const Config& config){

    // init event factory
    mEventFactory.init(config.eventPoolSizeBytes);

    // init event queue
    mEventQueue.init(config.eventQueueLength);

    // setup event forwarder
    mEventForwarder.setHandler([this](const Event* event){
        handleForwardedEvent(event);
    });

    // init all controllers
    onRegisterControllers(); // derived class must register controllers
    for(ControllerIfc* controller : mControllersSpan){
        controller->init(mEventFactory, mEventForwarder);
    }

    // init all behaviours
    onRegisterBehaviours(); // derived class must register behaviours
    for(BehaviourIfc* behaviour : mBehavioursSpan){
        behaviour->init(mEventFactory, mEventForwarder);
    }

    // init derived class
    onInit();
}

void SystemRuntimeAbs::start(){

    AssertExit(mBehavioursSpan.size() > 0, "SystemRuntimeAbs", "no behaviours registered");
    AssertExit(mControllersSpan.size() > 0, "SystemRuntimeAbs", "no controllers registered");

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

void SystemRuntimeAbs::setQueuedBehaviour(BehaviourIfc* behaviour){
    mQueuedBehaviour = behaviour;
}

void SystemRuntimeAbs::applyQueuedBehaviour() {
    if(mQueuedBehaviour){
        mActiveBehaviour = mQueuedBehaviour;
        mQueuedBehaviour = nullptr;
    }
}

bool SystemRuntimeAbs::hasQueuedBehaviour() const {
    return mQueuedBehaviour != nullptr;
}

BehaviourIfc* SystemRuntimeAbs::getActiveBehaviour() const {
    return mActiveBehaviour;
}

BehaviourIfc* SystemRuntimeAbs::getQueuedBehaviour() const {
    return mQueuedBehaviour;
}

void SystemRuntimeAbs::registerController(ControllerIfc* controller){
    const size_t maxIndex = MaxControllersCount - 1;
    const size_t nextIndex = mControllersSpan.size();
    AssertExit(nextIndex <= maxIndex, "SystemRuntimeAbs", "max controllers count exceeded");
    mControllersRawArray[nextIndex] = controller;
    mControllersSpan = Span<ControllerIfc*>(mControllersRawArray, mControllersSpan.size()+1);
}

Span<ControllerIfc*> SystemRuntimeAbs::getControllers(){
    return mControllersSpan;
}

void SystemRuntimeAbs::registerBehaviour(BehaviourIfc* behaviour){
    const size_t maxIndex = MaxBehavioursCount - 1;
    const size_t nextIndex = mBehavioursSpan.size();
    AssertExit(nextIndex <= maxIndex, "SystemRuntimeAbs", "max behaviours count exceeded");
    mBehavioursRawArray[nextIndex] = behaviour;
    mBehavioursSpan = Span<BehaviourIfc*>(mBehavioursRawArray, mBehavioursSpan.size()+1);
}

Span<BehaviourIfc*> SystemRuntimeAbs::getBehaviours(){
    return mBehavioursSpan;
}

void SystemRuntimeAbs::dispatchEvents(){
    const Event* event;
    while(mEventQueue.pop(event)){
        if(event == nullptr){
            TriggerDebug("SystemRuntimeAbs", "event is nullptr");
            continue;
        }
        onRouteEvent(event);
    }
    mEventFactory.clearDataPool();
}

void SystemRuntimeAbs::clearEventQueue(){
    mEventQueue.clear();
    mEventFactory.clearDataPool();
}

void SystemRuntimeAbs::handleForwardedEvent(const Event* event){
    if(!mEventQueue.push(event)){
        TriggerExit("SystemRuntimeAbs", "event queue is full");
    }
}

} // namespace