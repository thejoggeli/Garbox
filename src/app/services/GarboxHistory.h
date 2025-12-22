#pragma once

#include "core/util/history/TimeSeriesMulti.h"

namespace Garbox {

class GarboxHistory {
public:

    enum class SeriesIndex : uint8_t {
        Window_1min = 0,
        Window_5min = 0,
        Window_15min = 0,
        Window_1h,
        Window_6h,
        Window_1d,
        Window_4d,
        Count
    };

    static GarboxHistory& Instance();

    void reset();
    void temperatureSample(float temperature);
    void powerSample(float power);
    
    const TimeSeriesMulti& getTemperatureHistory() const;
    const TimeSeriesMulti& getPowerHistory() const;

    const TimeSeries& getTemperatureSeries(SeriesIndex index) const;
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

    TimeSeriesMulti mTemperatureHistory;
    TimeSeriesMulti mPowerHistory;

};

} // namespace
