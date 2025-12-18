// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HardwareInit.h"
#include "core/assert/Assert.h"
#include "generated/hardware/AdcInstances.h"
#include "generated/hardware/GpioInstances.h"
#include "generated/hardware/I2cInstances.h"
#include "generated/hardware/SpiInstances.h"
#include "generated/hardware/TimerInstances.h"
#include "generated/hardware/LedcInstances.h"

namespace Garbox {
    
static bool sInitialized = false;

void HardwareInit::Init(){
    AssertExit(!sInitialized, "HardwareInit", "already initialized");

    AdcInstances::Init();
    GpioInstances::Init();
    I2cInstances::Init();
    SpiInstances::Init();
    TimerInstances::Init();
    LedcInstances::Init();
    
    sInitialized = true;
}

} // namespace