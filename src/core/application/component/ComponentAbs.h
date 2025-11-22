#pragma once

#include "core/application/component/ComponentIfc.h"

namespace Garbox {

class ComponentAbs : public ComponentIfc {
public:

    ComponentAbs(ComponentType type, ComponentId id);

    ComponentDescriptor getComponentDescriptor() const final;
    ComponentId getComponentId() const final;
    ComponentType getComponentType() const final;

protected:

    const ComponentDescriptor mComponentDescriptor;

};

} // namespace