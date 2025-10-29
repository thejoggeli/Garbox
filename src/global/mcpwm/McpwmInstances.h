#pragma once

#include "core/hardware/mcpwm/McpwmPair.h"

namespace Garbox {

class McpwmInstances {
public:

    static void Init();

    static McpwmPair& GetPiezoPair();

};

} // namespace
