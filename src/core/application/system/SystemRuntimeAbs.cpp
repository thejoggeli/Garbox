#include "SystemRuntimeAbs.h"

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

    // derived class must register controllers
    onRegisterControllers();

    // init all controllers
    for(ControllerIfc* controller : mControllersSpan){
        controller->init(mEventFactory, mEventForwarder);
    }

    // init derived class
    onInit();
}

void SystemRuntimeAbs::start(){

    // start all controllers
    for(ControllerIfc* controller : mControllersSpan){
        controller->start();
    }

    // start derived class
    onStart();
}

void SystemRuntimeAbs::registerController(ControllerIfc* controller){
    const size_t maxIndex = MaxControllersCount - 1;
    const size_t nextIndex = mControllersSpan.size();
    AssertExit(nextIndex <= maxIndex, "SystemRuntimeAbs", "max controllers count exceeded");
    mControllersArray[nextIndex] = controller;
    mControllersSpan = Span<ControllerIfc*>(mControllersArray, mControllersSpan.size()+1);
}

Span<ControllerIfc*> SystemRuntimeAbs::getControllers(){
    return mControllersSpan;
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