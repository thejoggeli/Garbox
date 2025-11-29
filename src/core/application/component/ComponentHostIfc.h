#pragma once

#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ControllerId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class EventHeader;
class EventFactory;

class ComponentHostIfc {
public:

    virtual void requestEnableController(ControllerId id) = 0;
    virtual void requestDisableController(ControllerId id) = 0;
    virtual void requestChangeBehaviour(BehaviourId id) = 0;
    virtual void requestChangeScreen(ScreenId id) = 0;
    virtual void requestUpdateScreenNow() = 0;

    virtual void publishEvent(const EventHeader* header) = 0;
    virtual EventFactory& getEventFactory() = 0;
    virtual const RuntimeContext& getContext() const = 0;

};

} // namespace
