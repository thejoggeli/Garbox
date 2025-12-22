#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
 

#include "core/application/controller/ControllerAbs.h"

namespace Garbox {

class Runtime;

class DevtoolsControllerAbs : public ControllerAbs {
public:

    // component constructor
    DevtoolsControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onLoggingTick() = 0;

protected:

private:

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox