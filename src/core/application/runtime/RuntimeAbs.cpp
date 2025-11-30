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
    mEventQueue(config.eventQueueLength){
    // constructor body
}

void RuntimeAbs::registerControllers(std::initializer_list<ComponentAbs*> controllers){
    for(ComponentAbs* component : controllers){
        const ComponentType type = component->getComponentType(); 
        const bool valid = (type == ComponentType::Controller);
        AssertExit(valid, "RuntimeAbs", "expected controller", static_cast<uint32_t>(type));
    }
    mControllers.init(controllers);
}

void RuntimeAbs::registerBehaviours(std::initializer_list<ComponentAbs*> behaviours){
    for(ComponentAbs* component : behaviours){
        const ComponentType type = component->getComponentType(); 
        const bool valid = (type == ComponentType::Behaviour);
        AssertExit(valid, "RuntimeAbs", "expected behaviour", static_cast<uint32_t>(type));
    }
    mBehaviours.init(behaviours);
}

void RuntimeAbs::registerScreens(std::initializer_list<ComponentAbs*> screens){
    for(ComponentAbs* component : screens){
        const ComponentType type = component->getComponentType(); 
        const bool valid = (type == ComponentType::Screen);
        AssertExit(valid, "RuntimeAbs", "expected screen", static_cast<uint32_t>(type));
    }
    mScreens.init(screens);
}

void RuntimeAbs::init(){

    AssertExit(mControllers.isInitialized(), "RuntimeAbs", "controllers group not initialized");
    AssertExit(mBehaviours.isInitialized(), "RuntimeAbs", "behaviours group not initialized");
    AssertExit(mScreens.isInitialized(), "RuntimeAbs", "screens group not initialized");

    // init all controllers
    for(ComponentAbs* component : mControllers.getComponents()){
        component->init(*this);
    }
    mControllers.enableAllComponents();

    // init all behaviours
    for(ComponentAbs* component : mBehaviours.getComponents()){
        component->init(*this);
    }

    // init all screens
    for(ComponentAbs* component : mScreens.getComponents()){
        component->init(*this);
    }

    // init derived class
    onInit();
}

void RuntimeAbs::start(){

    // start all controllers
    for(ComponentAbs* component : mControllers.getComponents()){
        component->start();
    }

    // start all behaviours
    for(ComponentAbs* component : mBehaviours.getComponents()){
        component->start();
    }

    // start all screens
    for(ComponentAbs* component : mScreens.getComponents()){
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

void RuntimeAbs::clearEventQueue(){
    mEventQueue.releaseAll();
    mEventFactory.releaseDataPool();
}

EventFactory& RuntimeAbs::getEventFactory(){
    return mEventFactory;
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