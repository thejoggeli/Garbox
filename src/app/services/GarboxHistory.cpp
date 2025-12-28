#include "GarboxHistory.h"

#include "core/assert/Assert.h"

namespace Garbox {

static constexpr uint32_t MinutesToSeconds = 60;
static constexpr uint32_t HoursToSeconds = 3600;
static constexpr uint32_t DaysToSeconds = 24*3600;
static constexpr std::array<uint32_t, 8> Windows = {
    static_cast<uint32_t>(1)  * MinutesToSeconds,
    static_cast<uint32_t>(4)  * MinutesToSeconds,
    static_cast<uint32_t>(16) * MinutesToSeconds,
    static_cast<uint32_t>(1)  * HoursToSeconds,
    static_cast<uint32_t>(4)  * HoursToSeconds,
    static_cast<uint32_t>(12) * HoursToSeconds,
    static_cast<uint32_t>(1)  * DaysToSeconds,
    static_cast<uint32_t>(4)  * DaysToSeconds,
};

GarboxHistory& GarboxHistory::Instance(){
    static GarboxHistory sInstance;
    return sInstance;
}

GarboxHistory::GarboxHistory():
    mMeasuredTemperatureHistory(Windows.size()),
    mTargetTemperatureHistory(Windows.size()),
    mPowerHistory(Windows.size()){
    
    // register series
    constexpr TimeSeries::SamplingMode SamplingMode = TimeSeries::SamplingMode::Average;
    for(uint32_t window : Windows){
        mMeasuredTemperatureHistory.registerSeries(window, SampleCount, SamplingMode);
        mTargetTemperatureHistory.registerSeries(window, SampleCount, SamplingMode);
        mPowerHistory.registerSeries(window, SampleCount, SamplingMode);
    }
}

void GarboxHistory::reset(){
    mMeasuredTemperatureHistory.reset();
    mTargetTemperatureHistory.reset();
    mPowerHistory.reset();
}

void GarboxHistory::measuredTemperatureSample(int32_t temperature){
    mMeasuredTemperatureHistory.tick(temperature);
}

void GarboxHistory::targetTemperatureSample(int32_t temperature){
    mTargetTemperatureHistory.tick(temperature);
}

void GarboxHistory::powerSample(int32_t power){
    mPowerHistory.tick(power);
}

const TimeSeries& GarboxHistory::getMeasuredTemperatureSeries(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) > lastIndex){
        TriggerExit("GarboxHistory", "invalid series index", static_cast<uint8_t>(index));
    }
    return mMeasuredTemperatureHistory.getSeries(static_cast<uint8_t>(index));
}

const TimeSeries& GarboxHistory::getTargetTemperatureSeries(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) > lastIndex){
        TriggerExit("GarboxHistory", "invalid series index", static_cast<uint8_t>(index));
    }
    return mTargetTemperatureHistory.getSeries(static_cast<uint8_t>(index));
}

const TimeSeries& GarboxHistory::getPowerSeries(SeriesIndex index) const {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) > lastIndex){
        TriggerExit("GarboxHistory", "invalid series index", static_cast<uint8_t>(index));
    }
    return mPowerHistory.getSeries(static_cast<uint8_t>(index));
}

GarboxHistory::SeriesIndex GarboxHistory::PrevIndex(SeriesIndex index) {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) == 0){
        return static_cast<SeriesIndex>(lastIndex);
    }
    return static_cast<SeriesIndex>(static_cast<uint8_t>(index)-1);
}

GarboxHistory::SeriesIndex GarboxHistory::NextIndex(SeriesIndex index) {
    constexpr uint8_t lastIndex = static_cast<uint8_t>(SeriesIndex::Count) - 1;
    if(static_cast<uint8_t>(index) >= lastIndex){
        return static_cast<SeriesIndex>(0);
    }
    return static_cast<SeriesIndex>(static_cast<uint8_t>(index)+1);
}

} // namespace
