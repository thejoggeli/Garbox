#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"
#include "generated/components/controllers/display/DisplayControllerStates.h"

namespace Garbox {

class Runtime;

class DisplayControllerAbs : public ControllerAbs {
public:

    // component constructor
    DisplayControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onRenderTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onDisplayCommandEvent(const DisplayCommandEvent& event) = 0;

protected:

    DisplayControllerStates& states();

private:

    std::optional<DisplayControllerStates> mStates;

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox