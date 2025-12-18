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
    float getPwmProgressMicros() const;

    // write access (next state)
    void setMeasuredVoltage(float value);
    void setMeasuredCurrent(float value);
    void setPwmProgressMicros(float value);

private:

    struct Data {
        float measuredVoltage;
        float measuredCurrent;
        float pwmProgressMicros;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox