#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class BehaviourAbs : public ComponentAbs {
public:

    BehaviourAbs(ComponentId componentId, BehaviourId behaviourId);

    // unlike controllers, behaviours can be active or inactive
    void setActive(bool active);
    bool isActive() const { return mActive; }

    // get the behaviour id
    BehaviourId getBehaviourId() const { return mBehaviourId; }

protected:

    const BehaviourId mBehaviourId;

    // abstract methods for user of class
    virtual void onBecomeActive() = 0;
    virtual void onBecomeInactive() = 0;

    // behaviours can request to change the active behaviour
    void requestChangeBehaviour(BehaviourId behaviourId);

private:

    bool mActive = false;

};

} // namespace