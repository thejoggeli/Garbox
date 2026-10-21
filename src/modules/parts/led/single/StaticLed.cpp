#include "StaticLed.h"

#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/rtos/MutexGuard.h"

namespace Garbox {

StaticLed::StaticLed(Gpio& gpio) : mGpio(gpio){
    mMutex = xSemaphoreCreateMutex();
    AssertExit(mMutex != nullptr, "StaticLed", "mutex creation failed");
}

void StaticLed::init(){
    // nothing to do
}

void StaticLed::toggle(){
    MutexGuard guard(mMutex);
    mGpio.toggle();
}

void StaticLed::setEnabled(bool enabled){
    MutexGuard guard(mMutex);
    mGpio.writeLevel(enabled);
}

bool StaticLed::isEnabled() const {
    MutexGuard guard(mMutex);
    return mGpio.readLevel();
}

} // namespace
