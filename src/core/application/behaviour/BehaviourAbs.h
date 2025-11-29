#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class BehaviourAbs : public ComponentAbs {
public:

    BehaviourAbs(ComponentId componentId, BehaviourId behaviourId);

    // get the behaviour id
    BehaviourId getBehaviourId() const { return mBehaviourId; }

protected:

    const BehaviourId mBehaviourId;
    
};

} // namespace