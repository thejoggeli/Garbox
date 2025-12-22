#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
 

#include "core/application/controller/ControllerAbs.h"
#include "generated/components/controllers/heatpad/HeatpadControllerStates.h"

namespace Garbox {

class Runtime;

class HeatpadControllerAbs : public ControllerAbs {
public:

    // component constructor
    HeatpadControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onHeatpadCommandEvent(const HeatpadCommandEvent& event) = 0;

protected:

    HeatpadControllerStates& states();

private:

    std::optional<HeatpadControllerStates> mStates;

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox