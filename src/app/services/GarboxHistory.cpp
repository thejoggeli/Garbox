#include "GarboxHistory.h"

#include "core/assert/Assert.h"

namespace Garbox {

static constexpr uint32_t MinutesToSeconds = 60;
static constexpr uint32_t HoursToSeconds = 3600;
static constexpr uint32_t DaysToSeconds = 24*3600;
static constexpr std::array<uint32_t, 5> Windows = {
    15 * MinutesToSeconds,  // 15 minutes
    60 * MinutesToSeconds,  // 60 minutes
    6  * HoursToSeconds,    // 6 hours 
    1  * DaysToSeconds,     // 1 day
    2  * DaysToSeconds,     // 2 days
};

GarboxHistory& GarboxHistory::Instance(){
    static GarboxHistory sInstance;
    return sInstance;
}

GarboxHistory::GarboxHistory():
    mTemperatureHistory(Windows.size()),
    mPowerHistory(Windows.size()){
    
    // register series
    constexpr uint16_t SampleCount = 64;
    constexpr TimeSeries::SamplingMode SamplingMode = TimeSeries::SamplingMode::Average;
    for(uint32_t window : Windows){
        mTemperatureHistory.registerSeries(window, SampleCount, SamplingMode);
        mPowerHistory.registerSeries(window, SampleCount, SamplingMode);
    }
}

void GarboxHistory::temperatureSample(float temperature){
    mTemperatureHistory.tick(temperature);
}

void GarboxHistory::powerSample(float power){
    mPowerHistory.tick(power);
}

const TimeSeriesMulti& GarboxHistory::getPowerHistory() const {
    return mPowerHistory;
}

const TimeSeries& GarboxHistory::getTemperatureSeries(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) > lastIndex){
        TriggerExit("GarboxHistory", "invalid series index", static_cast<uint8_t>(index));
    }
    return mTemperatureHistory.getSeries(static_cast<uint8_t>(index));
}

const TimeSeries& GarboxHistory::getPowerSeries(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) > lastIndex){
        TriggerExit("GarboxHistory", "invalid series index", static_cast<uint8_t>(index));
    }
    return mPowerHistory.getSeries(static_cast<uint8_t>(index));
}

GarboxHistory::SeriesIndex GarboxHistory::prevIndex(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) == 0){
        return static_cast<SeriesIndex>(lastIndex);
    }
    return static_cast<SeriesIndex>(static_cast<uint8_t>(index)-1);
}

GarboxHistory::SeriesIndex GarboxHistory::nextIndex(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) >= lastIndex){
        return static_cast<SeriesIndex>(0);
    }
    return static_cast<SeriesIndex>(static_cast<uint8_t>(index)+1);
}

} // namespace
