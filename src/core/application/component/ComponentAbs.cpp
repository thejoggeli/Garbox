#include "ComponentAbs.h"

#include "core/application/component/ComponentAbs.h"
#include "core/assert/Assert.h"

namespace Garbox {

ComponentAbs::ComponentAbs(ComponentType type, ComponentId id) : mComponentDescriptor{type, id}{
    // nothing to do
}

ComponentDescriptor ComponentAbs::getComponentDescriptor() const {
    return mComponentDescriptor;
}

ComponentId ComponentAbs::getComponentId() const {
    return mComponentDescriptor.id;
}

ComponentType ComponentAbs::getComponentType() const {
    return mComponentDescriptor.type;
}

} // namespace
