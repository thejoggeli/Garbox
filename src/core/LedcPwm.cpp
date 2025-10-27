#include "LedcPwm.h"

#include "utils/MathUtils.h"

namespace Garbox {

LedcPwm::LedcPwm(
    uint32_t pin,
    Channel channel,
    uint32_t frequency,
    uint8_t resolutionBits) :
    // init members
    mPin(pin),
    mChannel(channel),
    mFrequency(frequency),
    mResolution(resolutionBits),
    mMaxDuty((1U << mResolution) - 1),
    mMaxDutyFloat(static_cast<float>(mMaxDuty)){
    // nothing to do
}

void LedcPwm::init() {
    ledcSetup(static_cast<uint8_t>(mChannel), mFrequency, mResolution);
    ledcAttachPin(mPin, static_cast<uint8_t>(mChannel));
}

void LedcPwm::setDutyRaw(uint32_t duty) {
    if (duty > mMaxDuty) duty = mMaxDuty;
    ledcWrite(static_cast<uint8_t>(mChannel), duty);
    mDuty = duty;
}

void LedcPwm::setDutyNormalized(float duty) {
    duty = MathUtils::clamp<float>(duty, 0.0f, 1.0f);
    uint32_t scaledDuty = static_cast<uint32_t>(duty * mMaxDutyFloat);
    setDutyRaw(scaledDuty);
}

uint32_t LedcPwm::getDuty() const {
    return mDuty;
}

uint32_t LedcPwm::getFrequency() const {
    return mFrequency;
}

uint32_t LedcPwm::getMaxDuty() const {
    return mMaxDuty;
}

}  // namespace Garbox
