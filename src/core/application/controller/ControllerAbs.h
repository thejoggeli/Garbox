#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "shared/types/ControllerId.h"

namespace Garbox {

class ControllerAbs : public ComponentAbs {
public:

    ControllerAbs(ComponentId componentId, ControllerId behaviourId);

    // get the behaviour id
    ControllerId getControllerId() const { return mControllerId; }

protected:

    const ControllerId mControllerId;

private:

};

} // namespace