// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class HeatpadProgressState final : public StateAbs {
public:
    HeatpadProgressState();

    // read access (published state)
    float getPwmProgressMicros() const;

    // read access (next state)
    float nextPwmProgressMicros() const;

    // read access (last state)
    float lastPwmProgressMicros() const;

    // write access (next state)
    void setPwmProgressMicros(float value);

private:

    struct Data {
        float pwmProgressMicros;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox