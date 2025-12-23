#include "TimeSeriesMulti.h"

#include "core/assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

TimeSeriesMulti::TimeSeriesMulti(uint32_t seriesCount) : mSeries(seriesCount) {
    // constructor body
}

void TimeSeriesMulti::registerSeries(
    uint32_t windowSeconds, 
    uint16_t sampleCount, 
    TimeSeries::SamplingMode samplingMode){
    // create time series
    AssertExit(!mSeries.full(), "TimeSeriesMulti", "series count reached");
    mSeries.emplace(windowSeconds, sampleCount, samplingMode);
}

void TimeSeriesMulti::reset(){
    for(TimeSeries& series : mSeries){
        series.reset();
    }
}

void TimeSeriesMulti::tick(int32_t value){
    for(TimeSeries& series : mSeries){
        series.tick(value);
    }
}

uint8_t TimeSeriesMulti::getSeriesCount() const {
    return mSeries.size();
}

const TimeSeries& TimeSeriesMulti::getSeries(uint8_t index) const {
    if(index >= mSeries.size()){
        TriggerExit("TimeSeriesMulti", "invalid index", index);
    }
    return mSeries[index];
}

} // namespace
