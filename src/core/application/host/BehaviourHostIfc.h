#pragma once

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/application/host/BaseHostIfc.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

class BehaviourHostIfc : public BaseHostIfc {
public:

    virtual void requestChangeBehaviour(ComponentId id) = 0;
    virtual BehaviourIfc* getActiveBehaviour() const = 0;

};

} // namespace
