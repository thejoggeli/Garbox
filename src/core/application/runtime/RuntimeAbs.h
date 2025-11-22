#pragma once 

#include "core/application/behaviour/BehaviourAbs.h"
#include "core/application/controller/ControllerAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "core/application/host/ControllerHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "core/util/container/Span.h"
#include "core/util/container/RingBufferHeap.h"

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

    RuntimeAbs();

    // disallow copy and move 
    RuntimeAbs(const RuntimeAbs&) = delete;
    RuntimeAbs& operator=(const RuntimeAbs&) = delete;
    RuntimeAbs(RuntimeAbs&&) = delete;
    RuntimeAbs& operator=(RuntimeAbs&&) = delete;

    void init(const Config& config);
    void start();

    // runtime context related methods
    void beginTickSequence();
    void incrementTickCount();

    // BaseHostIfc
    virtual void publishEvent(const EventHeader* header) final;
    virtual EventFactory& getEventFactory() final;
    virtual const RuntimeContext& getContext() const final;

    // BehaviourHostIfc
    void requestChangeBehaviour(BehaviourId id) final;
    BehaviourAbs* getActiveBehaviour() const final;

protected:

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
    virtual void onRegisterControllers() = 0;
    virtual void onRegisterBehaviours() = 0;
    virtual void onRouteEvent(const EventHeader* event) = 0;

private:

    // Context for controllers and behaviours to use.
    // RuntimeAbs only modifies this in init() and start().
    // For all other updates, the user must call e.g.
    // - incrementTickCount()
    // at the appropriate time
    RuntimeContext mContext;

    // members for events
    EventFactory mEventFactory;
    RingBufferHeap<const EventHeader*> mEventQueue; // store only pointer, events are owned by event factory

    // controllers array
    static constexpr size_t MaxControllersCount = 16;
    ControllerAbs* mControllersRawArray[MaxControllersCount];
    Span<ControllerAbs*> mControllersSpan;

    // behaviours array
    static constexpr size_t MaxBehavioursCount = 16;
    BehaviourAbs* mBehavioursRawArray[MaxBehavioursCount];
    Span<BehaviourAbs*> mBehavioursSpan;

    // behaviour active and queued
    BehaviourAbs* mActiveBehaviour = nullptr;
    BehaviourAbs* mQueuedBehaviour = nullptr;

};

} // namespace