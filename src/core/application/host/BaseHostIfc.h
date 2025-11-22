#pragma once

#include "core/application/runtime/RuntimeContext.h"

namespace Garbox {

class EventHeader;
class EventFactory;

class BaseHostIfc {
public:

    virtual void publishEvent(const EventHeader* header) = 0;
    virtual EventFactory& getEventFactory() = 0;
    virtual const RuntimeContext& getContext() const = 0;

};

} // namespace
