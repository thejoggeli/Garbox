#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/component/ComponentHostIfc.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/runtime/RuntimeContext.h"
#include "core/application/screen/ScreenAbs.h"
#include "core/util/container/heap/RingBufferHeap.h"
#include "core/util/container/Span.h"
#include "core/util/container/heap/VectorHeap.h"

namespace Garbox {

/**
 * Extend this class in the application
 */
class RuntimeAbs : public ComponentHostIfc {
public:

    struct Config {
        size_t numComponents;
        size_t eventPoolSizeBytes = 1024;
        size_t eventQueueLength = 64;
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
    void publishEvent(const EventHeader* header) final;
    EventFactory& getEventFactory() final;
    const RuntimeContext& getContext() const final;

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

    // event handling internal methods
    void dispatchEvents();
    void clearEventQueue();

    // behaviours internal methods
    void setQueuedBehaviour(BehaviourAbs* behaviour);
    void applyQueuedBehaviour(); 

    // screens internal methods
    void setQueuedScreen(ScreenAbs* screen);

    // to be implemented by user
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onRun() = 0;
    virtual void onRouteEvent(const EventHeader* event) = 0;
    virtual void onActiveBehaviourChanged() = 0;
    virtual void onActiveScreenChanged() = 0;

    // resolve specialized types
    virtual BehaviourAbs* resolveBehaviour(BehaviourId id) = 0;
    virtual ControllerAbs* resolveController(ControllerId id) = 0;
    virtual ScreenAbs* resolveScreen(ScreenId id) = 0;

private:

    // private because this is already called in requestUpdateScreenNow()
    void applyQueuedScreen(); 

    // component descriptor
    ComponentDescriptor mComponentDescriptor {ComponentType::Runtime, ComponentId::Runtime};

    // members for events
    EventFactory mEventFactory;
    RingBufferHeap<const EventHeader*> mEventQueue; // store only pointer, events are owned by event factory

    VectorHeap<ComponentAbs*> mComponents;
};

} // namespace