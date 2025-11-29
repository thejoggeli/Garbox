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

RuntimeAbs::RuntimeAbs(
    const Config& config,
    std::initializer_list<ComponentAbs*> controllers,
    std::initializer_list<ComponentAbs*> behaviours,
    std::initializer_list<ComponentAbs*> screens):
    // init members
    mEventFactory(config.eventPoolSizeBytes),
    mEventQueue(config.eventQueueLength),
    mComponents(controllers.size() + behaviours.size() + screens.size()),
    mControllers(controllers),
    mBehaviours(behaviours),
    mScreens(screens){
    // constructor body
}

void RuntimeAbs::init(){

    // derived class must register controllers, behaviours and ticks
    onRegister();

    // init all components
    for(ComponentAbs* component : mComponents){
        component->init(*this);
    }

    // init derived class
    onInit();
}

void RuntimeAbs::start(){

    AssertExit(mComponents.size() > 0, "RuntimeAbs", "no components registered");

    // start all components
    for(ComponentAbs* component : mComponents){
        component->start();
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
        mBehaviours.setComponentEnabled(mActiveBehaviour, false);
        event->oldBehaviour = mActiveBehaviour->getBehaviourId();
    }
    else {
        event->oldBehaviour = BehaviourId::Null;
    }
    mActiveBehaviour = mQueuedBehaviour;
    mQueuedBehaviour = nullptr;
    mBehaviours.setComponentEnabled(mActiveBehaviour, true);

    // send event
    onActiveBehaviourChanged();
    publishEvent(event.header());   
}

void RuntimeAbs::setQueuedScreen(ScreenAbs* screen){
    mQueuedScreen = screen;
}

void RuntimeAbs::applyQueuedScreen() {

    if(!mQueuedScreen){
        return;
    }

    if(mQueuedScreen == mActiveScreen){
        TriggerDebug("RuntimeAbs", "queued behaviour is already active");
        return;
    }

    // create event
    EventView event = mEventFactory.make<EventType::ActiveScreenChanged>(mComponentDescriptor);
    event->newScreen = mQueuedScreen->getScreenId();

    // set queued screen active
    if(mActiveScreen){
        mScreens.setComponentEnabled(mActiveScreen, false);
        event->oldScreen = mActiveScreen->getScreenId();
    }
    else {
        event->oldScreen = ScreenId::Null;
    }
    mActiveScreen = mQueuedScreen;
    mQueuedScreen = nullptr;
    mScreens.setComponentEnabled(mActiveScreen, true);

    // send event
    onActiveScreenChanged();
    publishEvent(event.header());   
}

void RuntimeAbs::registerComponent(ComponentAbs* component){
    AssertExit(!mComponents.full(), "RuntimeAbs", "max components count exceeded");
    mComponents.push(component);
}

void RuntimeAbs::publishEvent(const EventHeader* header){
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

void RuntimeAbs::requestEnableController(ControllerId id){
    // TODO implement
    // question: should be let the current tick finish before enabling?
}

void RuntimeAbs::requestDisableController(ControllerId id){
    // TODO implement
    // question: should be let the current tick finish before disabling?
}

void RuntimeAbs::requestChangeBehaviour(BehaviourId id){
    setQueuedBehaviour(resolveBehaviour(id));
}

void RuntimeAbs::requestChangeScreen(ScreenId id){
    setQueuedScreen(resolveScreen(id));
}

void RuntimeAbs::requestUpdateScreenNow(){
    if(mActiveScreen){
        mActiveScreen->updateScreen();
    }
}

} // namespace