// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class FanSampleState final : public StateAbs {
public:
    FanSampleState();

    // read access (published state)
    float getMeasuredRpm() const;

    // read access (next state)
    float nextMeasuredRpm() const;

    // write access (next state)
    void setMeasuredRpm(float value);

private:

    struct Data {
        float measuredRpm;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox