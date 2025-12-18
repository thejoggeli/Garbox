// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HardwareInit.h"
#include "core/assert/Assert.h"
#include "app_generated/hardware/AdcInstances.h"
#include "app_generated/hardware/GpioInstances.h"
#include "app_generated/hardware/I2cInstances.h"
#include "app_generated/hardware/SpiInstances.h"
#include "app_generated/hardware/TimerInstances.h"
#include "app_generated/hardware/LedcInstances.h"

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