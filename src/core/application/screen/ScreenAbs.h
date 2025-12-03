#pragma once 

#include "core/application/component/ComponentAbs.h"
#include "core/lvgl/LvglContext.h"
#include "core/lvgl/LvContainer.h"
#include "core/util/helpers/DirtyDispatcher.h"
#include "shared/types/BehaviourId.h"
#include "shared/types/ScreenId.h"

namespace Garbox {

class ScreenAbs : public ComponentAbs {
public:

    ScreenAbs(ComponentId componentId, ScreenId screenId, uint32_t dispatcherCapacity, LvglContext& lvgl);

    virtual void init(ComponentHostIfc& host) override;
    virtual void becomeEnabled() override;
    virtual void becomeDisabled() override;
    virtual void updateScreen();

    void setBackgroundColor(uint32_t color);

    // get the behaviour id
    ScreenId getScreenId() const { return mScreenId; }

protected:

    const ScreenId mScreenId;
    LvglContext& mLvgl;
    LvContainer mContainer;

    bool mFirstUpdate = true;
    uint32_t mDisplayWidth = 0;
    uint32_t mDisplayHeight = 0;

    virtual void onUpdateScreen() = 0;

    // register update handler
    void registerUpdateHandler(DirtyDispatcher::HandlerFunction handler, void* context);

    // number of times any apply-handler method was called
    uint32_t getDispatchedCount() const { return mDispatchedCount; }

    // dirty handling
    void markDirtyRaw(uint32_t index);
    bool isMarkedDirtyRaw(uint32_t index) const;

private:

    bool mActive = false; 
    DirtyDispatcher mDirtyDispatcher;
    uint32_t mDispatchedCount = 0;

};

} // namespace