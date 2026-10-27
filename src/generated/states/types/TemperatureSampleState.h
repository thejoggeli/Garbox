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
    uint64_t getSampleMicros() const;

    // read access (next state)
    float nextTemperatureCelcius() const;
    float nextHumidityRelative() const;
    uint64_t nextSampleMicros() const;

    // read access (last state)
    float lastTemperatureCelcius() const;
    float lastHumidityRelative() const;
    uint64_t lastSampleMicros() const;

    // write access (next state)
    void setTemperatureCelcius(float value);
    void setHumidityRelative(float value);
    void setSampleMicros(uint64_t value);

private:

    struct Data {
        float temperatureCelcius;
        float humidityRelative;
        uint64_t sampleMicros;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox