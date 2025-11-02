#include "Display.h"

#include <cmath>
#include "assert/Assert.h"
#include "global/PinConfig.h"
#include "global/bus/SpiInstances.h"
#include "global/ledc/LedcInstances.h"

namespace Garbox {

Display::Display():
    // init members
    mSpi(SpiInstances::GetSpiDma()),
    mBacklightPwm(LedcInstances::GetBacklightChannel()),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Display::init(){
    mTestTimer.start(2000_ms);

}

void Display::tick(){

    if(mTestTimer.isExpired()){
        mSpi.queue(nullptr, 0, this, handleTxComplete);
        mTestTimer.restart();
    }

    float const duty = std::sin(mTestTimer.getElapsedFraction() * static_cast<float>(M_TWOPI)) * 0.5f + 0.5f;
    mBacklightPwm.setDutyRelative(duty);

}

void Display::handleTxComplete(void* user){
    if(user == nullptr){
        AssertDebug(false, "Display::handleTxComplete()", "user == nullptr");
        return;
    }
    Display* self = (static_cast<Display*>(user));
}

} // namespace