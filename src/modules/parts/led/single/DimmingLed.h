#pragma once

#include <cstdint>

namespace Garbox {

class MathFunctionIfc;
class LedcChannel;

class DimmingLed {
public:

    DimmingLed(LedcChannel& ledcChannel);

    void init();

    void setBrightness(float brightness);
    void setCorrectionFunction(const MathFunctionIfc& function);

    float getBrightness() const;
    float getBrightnessCorrected() const;

    // disallow copy and move 
    DimmingLed(const DimmingLed&) = delete;
    DimmingLed& operator=(const DimmingLed&) = delete;
    DimmingLed(DimmingLed&&) = delete;
    DimmingLed& operator=(DimmingLed&&) = delete;

private:

    LedcChannel& mLedcChannel;
    const MathFunctionIfc* mCorrectionFunction = nullptr;
    float mBrightness = 0;
    float mBrightnessCorrected = 0;

    bool mInitialized;

};

} // namespace