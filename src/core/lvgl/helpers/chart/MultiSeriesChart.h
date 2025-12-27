#pragma once

#include <cstdint>

#include "core/util/container/heap/VectorHeap.h"
#include "core/lvgl/helpers/chart/TimeSeriesChartAdapter.h"
#include "core/lvgl/objects/LvChart.h"

namespace Garbox {

class TimeSeries;

class MultiSeriesChart {
public:

    MultiSeriesChart(
        LvChart& chart,
        uint8_t maxSeriesCount
    );

    void addSeries(
        uint32_t lineColor,
        int32_t yScale
    );

    void attach(
        uint8_t seriesIndex,
        const TimeSeries& timeSeries
    );

    void update(uint8_t seriesIndex);
    void updateAll();

    void clear(uint8_t seriesIndex);
    void clearAll();

    uint8_t getSeriesCount() const;
    LvChart& getLvChart() { return mChart; }

protected:

    LvChart& mChart;
    VectorHeap<TimeSeriesChartAdapter> mSeries;
};

} // namespace Garbox
