#pragma once

#include <functional>

namespace Garbox {

class EventFactory;
class Event;

class ControllerIfc {
public:

    using EventHandler = std::function<void(const Event& event)>;

    virtual void setup(ControllerId controllerId, EventFactory& eventFactory) = 0;
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void tick() = 0;
    virtual ControllerId getControllerId() = 0;
    virtual EventFactory& getEventFactory() = 0;

protected:

    virtual void sendEvent(Event* event) = 0;

};

} // namespace
