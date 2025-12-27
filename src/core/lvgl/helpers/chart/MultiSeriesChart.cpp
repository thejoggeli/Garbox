#include "MultiSeriesChart.h"

#include "core/assert/Assert.h"

namespace Garbox {

MultiSeriesChart::MultiSeriesChart(LvChart& chart, uint8_t maxSeriesCount):
    mChart(chart),
    mSeries(maxSeriesCount){
    // constructor body
}

void MultiSeriesChart::addSeries(uint32_t lineColor){
    AssertExit((mSeries.size() < mSeries.capacity()), "MultiSeriesChart", "series capacity exceeded");
    mSeries.emplace(mChart, lv_color_hex(lineColor));
}

void MultiSeriesChart::attach(uint8_t seriesIndex, const TimeSeries& timeSeries){
    AssertExit((seriesIndex < mSeries.size()), "MultiSeriesChart", "invalid series index");
    mSeries[seriesIndex].attach(timeSeries);
}

void MultiSeriesChart::update(uint8_t seriesIndex){
    AssertExit(seriesIndex < mSeries.size(), "MultiSeriesChart", "series index out of range");
    mSeries[seriesIndex].update();
}

void MultiSeriesChart::updateAll(){
    for(uint8_t i = 0; i < mSeries.size(); i++){
        mSeries[i].update();
    }
}

void MultiSeriesChart::clear(uint8_t seriesIndex){
    AssertExit(seriesIndex < mSeries.size(), "MultiSeriesChart", "series index out of range");
    mSeries[seriesIndex].clear();
}

void MultiSeriesChart::clearAll(){
    for(uint8_t i = 0; i < mSeries.size(); i++){
        mSeries[i].clear();
    }
}

uint8_t MultiSeriesChart::getSeriesCount() const{
    return static_cast<uint8_t>(mSeries.size());
}

LvChartSeries* MultiSeriesChart::getLvChartSeries(uint8_t seriesIndex){
    AssertExit(seriesIndex < mSeries.size(), "MultiSeriesChart", "series index out of range");
    return mSeries[seriesIndex].getLvChartSeries();
}

LvChart& MultiSeriesChart::getLvChart(){
    return mChart;
}

} // namespace Garbox
