#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DevtoolsControllerAbs : public ControllerAbs {
public:
    
    DevtoolsControllerAbs();

    void receiveTick(TickPhase phase) final {};
    void receiveEvent(const EventHeader* event) final {};

    // tick handlers
    virtual void onLoggingTick() = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox