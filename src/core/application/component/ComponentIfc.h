#pragma once

#include "core/application/component/ComponentDescriptor.h"

namespace Garbox {

class ComponentIfc {
public:

    virtual ComponentDescriptor getComponentDescriptor() const = 0;
    virtual ComponentId getComponentId() const = 0;
    virtual ComponentType getComponentType() const = 0;

};

} // namespace