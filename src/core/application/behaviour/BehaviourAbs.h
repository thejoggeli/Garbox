#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class BehaviourAbs : public ComponentAbs {
public:

    BehaviourAbs(ComponentId componentId, BehaviourId behaviourId);

    // set host
    void setBehaviourHost(BehaviourHostIfc& host);

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
    void requestChangeBehaviour(BehaviourId BehaviourId);

    // get the host of this behaviour
    BehaviourHostIfc* getBehaviourHost() { return static_cast<BehaviourHostIfc*>(mHost); }

private:

    bool mActive = false;

};

} // namespace