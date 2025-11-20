#pragma once

#include "core/types/ComponentDescriptor.h"

namespace Garbox {

class EventFactory;
class EventForwarder;
class EventHeader;

class ControllerIfc {
public:

    virtual void init(EventFactory& factory, EventForwarder& forwarder) = 0;
    virtual void start() = 0;

    virtual bool isInitialized() const = 0;
    virtual ComponentId getComponentId() const = 0;

protected:

    virtual void sendEvent(EventHeader* event) = 0;

};

} // namespace
