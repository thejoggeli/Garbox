#include "Display.h"

#include <cmath>
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"

namespace Garbox {

Display::Display() :
    // init members
    mBacklightPwm(LedcInstances::GetBacklightChannel()),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Display::init(){
    mTestTimer.start(2000);
}

void Display::tick(){

    if(mTestTimer.isExpired()){
        mTestTimer.restart();
    }

    float const duty = std::sin(mTestTimer.getElapsedFraction() * static_cast<float>(M_TWOPI)) * 0.5f + 0.5f;
    mBacklightPwm.setDutyRelative(duty);

}

} // namespace