#pragma once

#include "core/util/history/TimeSeriesMulti.h"

namespace Garbox {

class GarboxHistory {
public:

    static constexpr uint32_t SampleCount = 65;
    static constexpr int32_t TemperaturScaleFactor = 1024;
    static constexpr int32_t PowerScaleFactor = 1024;

    enum class SeriesIndex : uint8_t {
        Window_01min = 0,
        Window_05min = 0,
        Window_15min = 0,
        Window_01h,
        Window_06h,
        Window_01d,
        Window_04d,
        Count
    };

    static GarboxHistory& Instance();

    void reset();
    void measuredTemperatureSample(int32_t temperature);
    void targetTemperatureSample(int32_t temperature);
    void powerSample(int32_t power);

    const TimeSeries& getMeasuredTemperatureSeries(SeriesIndex index) const;
    const TimeSeries& getTargetTemperatureSeries(SeriesIndex index) const;
    const TimeSeries& getPowerSeries(SeriesIndex index) const;

    SeriesIndex prevIndex(SeriesIndex index) const;
    SeriesIndex nextIndex(SeriesIndex index) const;

private:

    // housekeeping 
    GarboxHistory();
    ~GarboxHistory() = default;
    GarboxHistory(const GarboxHistory&) = delete;
    GarboxHistory& operator=(const GarboxHistory&) = delete;
    GarboxHistory(GarboxHistory&&) = delete;
    GarboxHistory& operator=(GarboxHistory&&) = delete;

    TimeSeriesMulti mMeasuredTemperatureHistory;
    TimeSeriesMulti mTargetTemperatureHistory;
    TimeSeriesMulti mPowerHistory;

};

} // namespace
