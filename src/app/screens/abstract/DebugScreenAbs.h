#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "shared/types/EventType.h"

namespace Garbox {

class DebugScreenAbs : public ScreenAbs {
public:

    // constructor 
    DebugScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

protected:

    // make typed events 
    DisplayCommandEvent makeDisplayCommandEvent();

    // send typed events
    void sendEvent(const DisplayCommandEvent& event);

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

/********************/
/*** LVGL Section ***/
/********************/

protected:

    LvObject mRoot;
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

};

} // namespace Garbox