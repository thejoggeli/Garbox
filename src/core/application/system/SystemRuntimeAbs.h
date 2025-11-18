#pragma once 

#include "core/event/EventFactory.h"
#include "core/event/EventForwarder.h"
#include "core/util/container/Span.h"
#include "core/util/container/RingBufferHeap.h"

namespace Garbox {

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

    void init(const Config& config);
    void start();

protected:

    void registerController(ControllerIfc* controller);
    Span<ControllerIfc*> getControllers();

    void dispatchEvents();
    void clearEventQueue();

    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onRegisterControllers() = 0;
    virtual void onRouteEvent(const Event* event) = 0;

private:

    static constexpr size_t MaxControllersCount = 16;

    EventFactory mEventFactory;
    EventForwarder mEventForwarder;
    RingBufferHeap<const Event*> mEventQueue; // store only pointer, events are owned by event factory

    ControllerIfc* mControllersArray[MaxControllersCount];
    Span<ControllerIfc*> mControllersSpan;

    void handleForwardedEvent(const Event* event);

};

} // namespace