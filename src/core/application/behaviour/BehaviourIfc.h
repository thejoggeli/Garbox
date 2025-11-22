#pragma once 

#include "core/types/ComponentDescriptor.h"

namespace Garbox {

class BehaviourHostIfc;
class EventHeader;
class RuntimeContext;

class BehaviourIfc {
public:

    virtual void init(BehaviourHostIfc& host) = 0;
    virtual void start() = 0;

    virtual void setActive(bool active) = 0;
    virtual bool isActive() const = 0;
    virtual bool isInitialized() const = 0;
    virtual ComponentId getComponentId() const = 0;
    virtual const RuntimeContext* getContext() const = 0;
    virtual BehaviourHostIfc* getHost() = 0;

protected:

    virtual void sendEvent(EventHeader* event) = 0;

};

} // namespace