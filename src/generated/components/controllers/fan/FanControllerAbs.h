#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"
#include "generated/components/controllers/fan/FanControllerStates.h"

namespace Garbox {

class Runtime;

class FanControllerAbs : public ControllerAbs {
public:

    // component constructor
    FanControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;
    virtual void onOutputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onFanCommandEvent(const FanCommandEvent& event) = 0;

protected:

    FanControllerStates& states();

private:

    std::optional<FanControllerStates> mStates;

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox