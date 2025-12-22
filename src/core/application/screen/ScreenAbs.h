#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/util/helpers/DirtyDispatcher.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class ScreenAbs : public ComponentAbs {
public:

    ScreenAbs(ComponentId componentId, ScreenId screenId);

    virtual void init(ComponentHostIfc& host) override;
    virtual void becomeEnabled() override;
    virtual void becomeDisabled() override;
    virtual void render();

    // get the behaviour id
    ScreenId getScreenId() const { return mScreenId; }

protected:

    const ScreenId mScreenId;

    bool mFirstUpdate = true;

    virtual void onRender() = 0;

private:

    bool mActive = false; 

};

} // namespace