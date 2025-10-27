#include "Display.h"

#include "config/PinConfig.h"
#include "config/LedcPwmConfig.h"

namespace Garbox {

// Backlight Config
static constexpr uint32_t BacklightPwmFrequencyHz = 5000;
static constexpr uint32_t BacklightPwmResolutionBits = 10;

Display::Display() :
    // init members
    mBacklightPwm(PinConfig::DisplayBlk, LedcPwmConfig::DisplayBacklight, BacklightPwmFrequencyHz, BacklightPwmResolutionBits),
    mTestTimer() {
    // constructor body
    // nothing to do
}

void Display::init(){
    mBacklightPwm.init();
    mBacklightPwm.setDutyRaw(0);
    mTestTimer.start(2000);
}

void Display::tick(){

    if(mTestTimer.isExpired()){
        mTestTimer.restart();
    }

    float const duty = std::sin(mTestTimer.getElapsedFraction() * static_cast<float>(M_TWOPI)) * 0.5f + 0.5f;
    mBacklightPwm.setDutyNormalized(duty);

}

} // namespace