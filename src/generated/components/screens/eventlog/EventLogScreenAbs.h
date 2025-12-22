#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/screen/ScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"

namespace Garbox {

class Runtime;

class EventLogScreenAbs : public ScreenAbs {
public:

    // component constructor
    EventLogScreenAbs();

    // event handler (to be implmeneted by user)
    virtual void onEvent(const EventHeader* header) = 0;

protected:

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::sendEventToHost;

    friend class Runtime;

public:

    // init override from component
    void init(ComponentHostIfc& host) final;
    void render() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

protected:

    LvObject mRoot;
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

};

} // namespace Garbox