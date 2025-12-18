// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class DisplayStatusState final : public StateAbs {
public:
    DisplayStatusState();

    // read access (published state)
    float getBrightness() const;

    // read access (next state)
    float nextBrightness() const;

    // write access (next state)
    void setBrightness(float value);

private:

    struct Data {
        float brightness;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox