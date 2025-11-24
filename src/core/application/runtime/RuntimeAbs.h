#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "core/application/host/ControllerHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "core/util/container/ringbuffer/HeapRingBuffer.h"
#include "core/util/container/Span.h"

namespace Garbox {

/**
 * Extend this class in the application
 */
class RuntimeAbs : public BehaviourHostIfc, public ControllerHostIfc {
public:

    struct Config {
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

    // BaseHostIfc
    virtual void publishEvent(const EventHeader* header) final;
    virtual EventFactory& getEventFactory() final;
    virtual const RuntimeContext& getContext() const final;

    // BehaviourHostIfc
    void requestChangeBehaviour(BehaviourId id) final;
    BehaviourAbs* getActiveBehaviour() const final;

protected:

    // Context for controllers and behaviours to use.
    // The deriving class update the values at appropriate times
    RuntimeContext mContext;

    // behaviour active and queued
    BehaviourAbs* mActiveBehaviour = nullptr;
    BehaviourAbs* mQueuedBehaviour = nullptr;

    // controllers setup
    void registerController(ControllerAbs* controller);
    Span<ControllerAbs*> getControllers();
    virtual ControllerAbs* resolveController(ControllerId id) = 0;

    // behaviours setup
    void registerBehaviour(BehaviourAbs* behaviour);
    Span<BehaviourAbs*> getBehaviours();
    virtual BehaviourAbs* resolveBehaviour(BehaviourId id) = 0;

    // event handling internal methods
    void dispatchEvents();
    void clearEventQueue();

    // behaviours internal methods
    void setQueuedBehaviour(BehaviourAbs* behaviour);
    void applyQueuedBehaviour(); 
    bool hasQueuedBehaviour() const;

    // to be implemented by user
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onRun() = 0;
    virtual void onRegister() = 0;
    virtual void onRouteEvent(const EventHeader* event) = 0;

private:

    // component descriptor
    ComponentDescriptor mComponentDescriptor {ComponentType::Runtime, ComponentId::Runtime};

    // members for events
    EventFactory mEventFactory;
    HeapRingBuffer<const EventHeader*> mEventQueue; // store only pointer, events are owned by event factory

    // controllers array
    static constexpr size_t MaxControllersCount = 16;
    ControllerAbs* mControllersRawArray[MaxControllersCount];
    Span<ControllerAbs*> mControllersSpan;

    // behaviours array
    static constexpr size_t MaxBehavioursCount = 16;
    BehaviourAbs* mBehavioursRawArray[MaxBehavioursCount];
    Span<BehaviourAbs*> mBehavioursSpan;

};

} // namespace