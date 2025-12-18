// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class TemperatureSampleState final : public StateAbs {
public:
    TemperatureSampleState();

    // read access (published state)
    float getTemperatureCelcius() const;
    float getHumidityRelative() const;

    // read access (next state)
    float nextTemperatureCelcius() const;
    float nextHumidityRelative() const;

    // write access (next state)
    void setTemperatureCelcius(float value);
    void setHumidityRelative(float value);

private:

    struct Data {
        float temperatureCelcius;
        float humidityRelative;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox