#pragma once

#include <cstdint>
#include "core/lvgl/objects/LvChart.h"
#include "core/util/container/Span.h"

namespace Garbox {

class TimeSeries;

class TimeSeriesChartAdapter {
public:

    TimeSeriesChartAdapter(LvChart& chart, lv_color_t lineColor);

    void attach(const TimeSeries& timeSeries);
    void update();
    void clear();

    LvChartSeries* getLvChartSeries();

private:

    void pushAllSamples();
    void pushNewSamples();

    LvChart& mChart;
    lv_chart_series_t* mSeries = nullptr;
    const TimeSeries* mTimeSeries = nullptr;
    uint32_t mLastSeenWriteSequence = 0;
};

} // namespace Garbox
