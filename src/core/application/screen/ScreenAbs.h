#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class ScreenAbs : public ComponentAbs {
public:

    ScreenAbs(ComponentId componentId, ScreenId screenId);

    // unlike controllers, behaviours can be active or inactive
    void setActive(bool active);
    bool isActive() const { return mActive; }

    // get the behaviour id
    ScreenId getScreenId() const { return mScreenId; }

protected:

    const ScreenId mScreenId;

    // abstract methods for user of class
    virtual void onBecomeActive() = 0;
    virtual void onBecomeInactive() = 0;

    void requestChangeBehaviour(BehaviourId screenId);
    void requestChangeScreen(ScreenId screenId);

private:

    bool mActive = false;

};

} // namespace