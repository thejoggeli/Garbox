#pragma once

#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/ControllerId.h"

namespace Garbox {

class ControllerHostIfc;

class ControllerIfc {
public:

    virtual void init(ControllerHostIfc& host) = 0;
    virtual void start() = 0;

    virtual bool isInitialized() const = 0;

    virtual ControllerId getControllerId() const = 0;
    virtual const RuntimeContext* getContext() const = 0;
    virtual ControllerHostIfc* getHost() = 0;

};

} // namespace
