#pragma once 

#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/BehaviourId.h"

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

    virtual BehaviourId getControllerId() const = 0;
    virtual const RuntimeContext* getContext() const = 0;
    virtual BehaviourHostIfc* getHost() = 0;

};

} // namespace