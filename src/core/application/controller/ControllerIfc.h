#pragma once

#include "core/application/component/ComponentAbs.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/ControllerId.h"

namespace Garbox {

class ControllerHostIfc;

class ControllerIfc : public ComponentAbs {
public:

    ControllerIfc(ComponentId id) : ComponentAbs(ComponentType::Controller, id) {}

    virtual void init(ControllerHostIfc& host) = 0;
    virtual void start() = 0;

    virtual bool isInitialized() const = 0;

    virtual ControllerId getControllerId() const = 0;
    virtual const RuntimeContext* getContext() const = 0;
    virtual ControllerHostIfc* getHost() = 0;

};

} // namespace
