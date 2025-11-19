#pragma once 

#include "core/application/event/EventFactory.h"
#include "core/application/event/EventForwarder.h"
#include "core/util/container/Span.h"
#include "core/util/container/RingBufferHeap.h"

namespace Garbox {

class BehaviourIfc;
class ControllerIfc;

/**
 * Extend this class in the application
 */
class SystemRuntimeAbs {
public:

    struct Config {
        size_t eventPoolSizeBytes = 1024;
        size_t eventQueueLength = 64;
    };

    SystemRuntimeAbs();

    // disallow copy and move 
    SystemRuntimeAbs(const SystemRuntimeAbs&) = delete;
    SystemRuntimeAbs& operator=(const SystemRuntimeAbs&) = delete;
    SystemRuntimeAbs(SystemRuntimeAbs&&) = delete;
    SystemRuntimeAbs& operator=(SystemRuntimeAbs&&) = delete;

    void init(const Config& config);
    void start();

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

    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onRegisterControllers() = 0;
    virtual void onRegisterBehaviours() = 0;
    virtual void onRouteEvent(const Event* event) = 0;

private:

    // members for events
    EventFactory mEventFactory;
    EventForwarder mEventForwarder;
    RingBufferHeap<const Event*> mEventQueue; // store only pointer, events are owned by event factory

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

    void handleForwardedEvent(const Event* event);

};

} // namespace