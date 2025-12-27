#include "TimeSeriesChartAdapter.h"

#include "core/assert/Assert.h"
#include "core/util/history/TimeSeries.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

TimeSeriesChartAdapter::TimeSeriesChartAdapter(
    LvChart& chart,
    lv_color_t lineColor,
    int32_t yScale):
    // initialize members
    mChart(chart),
    mSeries(chart.addSeries(lineColor)),
    mYScale(yScale){
    // constructor body
}

void TimeSeriesChartAdapter::attach(const TimeSeries& timeSeries){
    mTimeSeries = &timeSeries;
    mLastSeenWriteSequence = 0;
    mChart.clearSeries(mSeries);
    pushAllSamples();
}

void TimeSeriesChartAdapter::update(){
    if(mTimeSeries == nullptr){
        return;
    }
    pushNewSamples();
}

void TimeSeriesChartAdapter::clear(){
    mChart.clearSeries(mSeries);
    mTimeSeries = nullptr;
    mLastSeenWriteSequence = 0;
}

lv_chart_series_t* TimeSeriesChartAdapter::getChartSeries(){
    return mSeries;
}

void TimeSeriesChartAdapter::pushAllSamples(){

    auto it = mTimeSeries->iterateOldestToNewest();

    while(it.hasNext()){
        const int32_t value = it.next();
        mChart.setNextValue(
            mSeries,
            value * mYScale
        );
    }

    mLastSeenWriteSequence = mTimeSeries->getWriteSequence();
}

void TimeSeriesChartAdapter::pushNewSamples(){

    if(mTimeSeries->getWriteSequence() == mLastSeenWriteSequence){
        return;
    }

    auto it = mTimeSeries->iterateSince(mLastSeenWriteSequence);

    while(it.hasNext()){
        const int32_t value = it.next();
        mChart.setNextValue(
            mSeries,
            value * mYScale
        );
    }

    mLastSeenWriteSequence = it.getNewWriteSequence();
}

} // namespace Garbox
