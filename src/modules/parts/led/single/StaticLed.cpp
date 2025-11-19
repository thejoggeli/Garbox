#include "StaticLed.h"

#include "core/assert/Assert.h"
#include "core/hardware/gpio/Gpio.h"
#include "core/rtos/LockGuard.h"

namespace Garbox {

StaticLed::StaticLed(Gpio& gpio) : mGpio(gpio){
    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "StaticLed", "mutex creation failed");
}

void StaticLed::init(){
    // nothing to do
}

void StaticLed::toggle(){
    LockGuard guard(mMutex);
    mGpio.toggle();
}

void StaticLed::setEnabled(bool enabled){
    LockGuard guard(mMutex);
    mGpio.writeLevel(enabled);
}

bool StaticLed::isEnabled() const {
    LockGuard guard(mMutex);
    return mGpio.readLevel();
}

} // namespace
