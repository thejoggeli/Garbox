#pragma once

#include <cstdint>

namespace Garbox {

class FunctionIfc;
class LedcChannel;

class DimmingLed {
public:

    DimmingLed(LedcChannel& ledcChannel);

    void init();

    void setBrightness(float brightness);
    void setCorrectionFunction(const FunctionIfc& function);

    float getBrightness() const;
    float getBrightnessCorrected() const;

private:

    LedcChannel& mLedcChannel;
    const FunctionIfc* mCorrectionFunction = nullptr;
    float mBrightness = 0;
    float mBrightnessCorrected = 0;

    bool mInitialized;

};

} // namespace