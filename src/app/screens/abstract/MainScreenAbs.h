#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "app/guis/MainScreenGui.h"
#include "core/application/screen/ScreenAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class MainScreenAbs : public ScreenAbs {
public:

    // constructor 
    MainScreenAbs();

    // init override from component
    void init(ComponentHostIfc& host) final;
    void updateScreen() final;
    void becomeEnabled() final;
    void becomeDisabled() final;

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommand(const DisplayCommandEvent& event) = 0;
    virtual void onFanCommand(const FanCommandEvent& event) = 0;
    virtual void onHeatpadCommand(const HeatpadCommandEvent& event) = 0;

protected:

private:

    // hide event methods
    using ScreenAbs::makeEvent;
    using ScreenAbs::publishEvent;

/*******************/
/*** GUI Section ***/
/*******************/

private:

    MainScreenGui mGui;

protected:

    MainScreenGui::Objects& gui(); 
    const uint32_t mScreenWidth = 0;
    const uint32_t mScreenHeight = 0;

    void setBackgroundColor(uint32_t color);

};

} // namespace Garbox