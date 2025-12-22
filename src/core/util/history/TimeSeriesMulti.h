#pragma once

#include <cstdint>
#include "core/util/container/heap/VectorHeap.h"
#include "core/util/history/TimeSeries.h"

namespace Garbox {

class TimeSeriesMulti {
public:

    TimeSeriesMulti(uint32_t numSeries);

    void registerSeries(
        uint32_t windowSeconds, 
        uint16_t sampleCount, 
        TimeSeries::SamplingMode samplingMode
    );

    void reset();
    void tick(float value);

    uint8_t getSeriesCount() const;
    const TimeSeries& getSeries(uint8_t index) const;

private:

    VectorHeap<TimeSeries> mSeries;

};

} // namespace
