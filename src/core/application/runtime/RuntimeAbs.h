#pragma once 

#include "core/application/event/EventFactory.h"
#include "core/application/event/EventForwarder.h"
#include "core/application/runtime/RuntimeContext.h"
#include "core/util/container/Span.h"
#include "core/util/container/RingBufferHeap.h"

namespace Garbox {

class BehaviourIfc;
class ControllerIfc;

/**
 * Extend this class in the application
 */
class RuntimeAbs {
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

    void incrementTickCount();
    const RuntimeContext& getContext() const;

protected:

    void registerController(ControllerIfc* controller);
    Span<ControllerIfc*> getControllers();

    void registerBehaviour(BehaviourIfc* behaviour);
    Span<BehaviourIfc*> getBehaviours();

    void setQueuedBehaviour(BehaviourIfc* behaviour);
    void applyQueuedBehaviour(); 
    bool hasQueuedBehaviour() const;
    BehaviourIfc* getActiveBehaviour() const;
    BehaviourIfc* getQueuedBehaviour() const;

    void dispatchEvents();
    void clearEventQueue();

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
    EventForwarder mEventForwarder;
    RingBufferHeap<const EventHeader*> mEventQueue; // store only pointer, events are owned by event factory

    // controllers array
    static constexpr size_t MaxControllersCount = 16;
    ControllerIfc* mControllersRawArray[MaxControllersCount];
    Span<ControllerIfc*> mControllersSpan;

    // behaviours array
    static constexpr size_t MaxBehavioursCount = 16;
    BehaviourIfc* mBehavioursRawArray[MaxControllersCount];
    Span<BehaviourIfc*> mBehavioursSpan;

    // behaviour active and queued
    BehaviourIfc* mActiveBehaviour = nullptr;
    BehaviourIfc* mQueuedBehaviour = nullptr;

    void handleForwardedEvent(const EventHeader* event);

};

} // namespace