#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/ControllerHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/ControllerId.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class ControllerAbs : public ComponentAbs {
public:

    ControllerAbs(ComponentId componentId, ControllerId behaviourId);

    // setup method
    void init(ControllerHostIfc& host);

    // get the host of this behaviour
    ControllerHostIfc* getControllerHost() { return static_cast<ControllerHostIfc*>(mHost); }

    // get the behaviour id
    ControllerId getControllerId() const { return mControllerId; }

private:

    ControllerId mControllerId;

};

} // namespace