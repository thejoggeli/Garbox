#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "core/application/controller/ControllerAbs.h"
#include "shared/types/EventType.h"

namespace Garbox {

class I2cPartsControllerAbs : public ControllerAbs {
public:

    // component constructor
    I2cPartsControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onInputTick() = 0;

    // event handlers (to be implmeneted by user)
    virtual void onButtonStateChanged(const ButtonStateChangedEvent& event) = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::publishEvent;

};

} // namespace Garbox