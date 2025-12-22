#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"
#include "generated/components/controllers/i2cparts/I2cPartsControllerStates.h"

namespace Garbox {

class Runtime;

class I2cPartsControllerAbs : public ControllerAbs {
public:

    // component constructor
    I2cPartsControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onButtonEvent(const ButtonEvent& event) = 0;

protected:

    I2cPartsControllerStates& states();

private:

    std::optional<I2cPartsControllerStates> mStates;

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox