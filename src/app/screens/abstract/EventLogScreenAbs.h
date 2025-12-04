#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvContainer.h"
#include "shared/types/EventType.h"

namespace Garbox {

class EventLogScreenAbs : public ScreenAbs {
public:

    // constructor 
    EventLogScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

    // event handler (to be implmeneted by user)
    virtual void onEvent(const EventHeader* header) = 0;

protected:

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

/********************/
/*** LVGL Section ***/
/********************/

protected:

    LvContainer mContainer;
    const uint32_t mDisplayWidth = 0;
    const uint32_t mDisplayHeight = 0;

    void setBackgroundColor(uint32_t color);

};

} // namespace Garbox