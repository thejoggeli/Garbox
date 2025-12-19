// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class HeatpadSampleState final : public StateAbs {
public:
    HeatpadSampleState();

    // read access (published state)
    float getMeasuredVoltage() const;
    float getMeasuredCurrent() const;

    // read access (next state)
    float nextMeasuredVoltage() const;
    float nextMeasuredCurrent() const;

    // read access (last state)
    float lastMeasuredVoltage() const;
    float lastMeasuredCurrent() const;

    // write access (next state)
    void setMeasuredVoltage(float value);
    void setMeasuredCurrent(float value);

private:

    struct Data {
        float measuredVoltage;
        float measuredCurrent;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox