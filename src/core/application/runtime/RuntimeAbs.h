#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/component/ComponentHostIfc.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/runtime/RuntimeContext.h"
#include "core/application/screen/ScreenAbs.h"
#include "core/application/state/StateAbs.h"
#include "core/application/state/StateHostIfc.h"
#include "core/util/container/heap/RingBufferHeap.h"
#include "core/util/container/Span.h"
#include "core/util/container/heap/VectorHeap.h"

namespace Garbox {

/**
 * Extend this class in the application
 */
class RuntimeAbs : public ComponentHostIfc, public StateHostIfc {
public:

    struct Config {
        size_t numComponents;
        size_t numStates;
        size_t eventPoolSizeBytes = 1024;
        size_t eventQueueLength = 64;
        size_t maxDispatchRecursionDepth = 3;
    };

    RuntimeAbs(const Config& config);

    // disallow copy and move 
    RuntimeAbs(const RuntimeAbs&) = delete;
    RuntimeAbs& operator=(const RuntimeAbs&) = delete;
    RuntimeAbs(RuntimeAbs&&) = delete;
    RuntimeAbs& operator=(RuntimeAbs&&) = delete;

    void init();
    void start();
    void run();

    // ComponentHostIfc
    void requestChangeBehaviour(BehaviourId id) final;
    void requestChangeScreen(ScreenId id) final;
    void requestUpdateScreenNow() final;
    void receiveEvent(const EventHeader* header) final;
    EventFactory& getEventFactory() final;
    const RuntimeContext& getContext() const final;

    // StateHostIfc
    void markStateDirty(StateAbs* state) final;

protected:

    // Context for controllers and behaviours to use.
    // The deriving class update the values at appropriate times
    RuntimeContext mContext;

    // behaviour active and queued
    BehaviourAbs* mActiveBehaviour = nullptr;
    BehaviourAbs* mQueuedBehaviour = nullptr;

    // screen active and queued
    ScreenAbs* mActiveScreen = nullptr;
    ScreenAbs* mQueuedScreen = nullptr;

    // components setup
    void registerComponent(ComponentAbs* component);

    // states setup
    void registerState(StateAbs* state);

    // dispatch states and events until a stable state is reached 
    void dispatch();

    // behaviours internal methods
    void setQueuedBehaviour(BehaviourAbs* behaviour);
    void applyQueuedBehaviour(); 

    // screens internal methods
    void setQueuedScreen(ScreenAbs* screen);

    // to be implemented by user
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onRun() = 0;
    virtual void onRouteStateChanged(const StateAbs& state) = 0;
    virtual void onRouteEvent(const EventHeader* event) = 0;
    virtual void onActiveBehaviourChanged() = 0;
    virtual void onActiveScreenChanged() = 0;

    // resolve specialized types
    virtual BehaviourAbs* resolveBehaviour(BehaviourId id) = 0;
    virtual ControllerAbs* resolveController(ControllerId id) = 0;
    virtual ScreenAbs* resolveScreen(ScreenId id) = 0;

private:

    const size_t mMaxDispatchRecursionDepth;

    // state and event handling internal methods
    void dispatchStates();
    void dispatchEvents();

    // private because this is already called in requestUpdateScreenNow()
    void applyQueuedScreen(); 

    // component descriptor
    ComponentDescriptor mComponentDescriptor {ComponentType::Runtime, ComponentId::Runtime};

    // members for events
    EventFactory mEventFactory;
    RingBufferHeap<const EventHeader*> mEventQueue; // store only pointer, events are owned by event factory

    // components
    VectorHeap<ComponentAbs*> mComponents;

    // states
    VectorHeap<StateAbs*> mStates;
    VectorHeap<StateAbs*> mDirtyStates; 
    // second state queue is required to allow issuing new state changes during dispatch
    VectorHeap<StateAbs*> mStateUpdatesPending;  
};

} // namespace