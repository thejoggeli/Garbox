#include "McpwmInstances.h"

#include "McpwmConfig.h"

namespace Garbox {

void McpwmInstances::Init() {

    GetPiezoPair().setup(
        McpwmConfig::PiezoUnit,
        McpwmConfig::PiezoTimer,
        McpwmConfig::PiezoPinA,
        McpwmConfig::PiezoPinB,
        McpwmConfig::PiezoInitialFrequency,
        McpwmConfig::PiezoInitialDutyPercent
    );

}

McpwmPair& McpwmInstances::GetPiezoPair() { static McpwmPair instance; return instance; }


} // namespace
