// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HardwareInit.h"
#include "core/assert/Assert.h"
#include "app/generated/hardware/AdcInstances.h"
#include "app/generated/hardware/GpioInstances.h"
#include "app/generated/hardware/I2cInstances.h"
#include "app/generated/hardware/SpiInstances.h"
#include "app/generated/hardware/TimerInstances.h"
#include "app/generated/hardware/LedcInstances.h"

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