#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class ScreenAbs : public ComponentAbs {
public:

    ScreenAbs(ComponentId componentId, ScreenId screenId);

    void updateScreen();

    // get the behaviour id
    ScreenId getScreenId() const { return mScreenId; }

protected:

    const ScreenId mScreenId;

    // abstract methods for user implementation
    virtual void onUpdateScreen() = 0;

private:

    bool mActive = false;

};

} // namespace