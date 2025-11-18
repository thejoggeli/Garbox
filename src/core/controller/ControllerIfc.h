#pragma once

#include <functional>
#include "shared/types/ControllerId.h"

namespace Garbox {

class EventFactory;
class EventForwarder;
class Event;

class ControllerIfc {
public:

    using EventHandler = std::function<void(const Event& event)>;

    virtual void init(EventFactory& factory, EventForwarder& forwarder) = 0;
    virtual void start() = 0;
    virtual ControllerId getControllerId() = 0;
    virtual EventFactory& getEventFactory() = 0;

protected:

    virtual void sendEvent(Event* event) = 0;

};

} // namespace
