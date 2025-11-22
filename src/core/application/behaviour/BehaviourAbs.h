#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class BehaviourAbs : public ComponentAbs {
public:

    BehaviourAbs(ComponentId componentId, BehaviourId behaviourId);

    // setup method
    void init(BehaviourHostIfc& host);

    // unlike controllers, behaviours can be active or inactive
    // the setActive must only be called by runtime
    void setActive(bool active);
    bool isActive() const { return mActive; }

    // get the host of this behaviour
    BehaviourHostIfc* getBehaviourHost() { return static_cast<BehaviourHostIfc*>(mHost); }

    // get the behaviour id
    BehaviourId getBehaviourId() const { return mBehaviourId; }

protected:

    // abstract methods for user of class
    virtual void onBecomeActive() = 0;
    virtual void onBecomeInactive() = 0;

private:

    BehaviourId mBehaviourId;
    bool mActive = false;

};

} // namespace