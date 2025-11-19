#pragma once 

#include "shared/types/ComponentId.h"
#include "shared/types/ComponentType.h"

namespace Garbox {

struct ComponentDescriptor {
    ComponentType type;
    ComponentId id;
};

} // namespace