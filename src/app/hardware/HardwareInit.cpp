// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "HardwareInit.h"
#include "core/assert/Assert.h"
#include "app/hardware/AdcInstances.h"
#include "app/hardware/GpioInstances.h"
#include "app/hardware/I2cInstances.h"
#include "app/hardware/SpiInstances.h"
#include "app/hardware/TimerInstances.h"
#include "app/hardware/LedcInstances.h"

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