#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/application/host/ControllerHostIfc.h"
#include "shared/types/ControllerId.h"

namespace Garbox {

class ControllerAbs : public ComponentAbs {
public:

    ControllerAbs(ComponentId componentId, ControllerId behaviourId);

    // setup method
    void init(ControllerHostIfc& host);

    // get the behaviour id
    ControllerId getControllerId() const { return mControllerId; }

protected:

    const ControllerId mControllerId;

    // get the host of this behaviour
    ControllerHostIfc* getControllerHost() { return static_cast<ControllerHostIfc*>(mHost); }

private:

};

} // namespace