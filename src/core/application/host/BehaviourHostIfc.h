#pragma once

#include "core/application/host/ComponentHostIfc.h"
#include "shared/types/BehaviourId.h"
    
namespace Garbox {

class BehaviourAbs;

class BehaviourHostIfc : public ComponentHostIfc {
public:

    virtual void requestChangeBehaviour(BehaviourId id) = 0;
    virtual BehaviourAbs* getActiveBehaviour() const = 0;

};

} // namespace
