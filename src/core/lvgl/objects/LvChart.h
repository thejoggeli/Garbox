#pragma once

#include <lvgl.h>
#include "LvObject.h"

namespace Garbox {

using LvChartSeries = lv_chart_series_t;

class LvChart : public LvObject {
public:
    LvChart(LvObject& parent);
    LvChart(lv_obj_t* raw);

    // ==============================================================================
    // type & configuration
    // ==============================================================================
    void setType(lv_chart_type_t type);
    void setPointCount(uint32_t count);
    void setUpdateMode(lv_chart_update_mode_t mode);
    void setDivLineCount(uint32_t hDiv, uint32_t vDiv);
    void setHorDivLineCount(uint32_t count);
    void setVerDivLineCount(uint32_t count);

    lv_chart_type_t getType() const;
    uint32_t getPointCount() const;

    // ==============================================================================
    // axis & ranges
    // ==============================================================================
    void setAxisRange(lv_chart_axis_t axis, int32_t min, int32_t max);
    void setAxisMinValue(lv_chart_axis_t axis, int32_t min);
    void setAxisMaxValue(lv_chart_axis_t axis, int32_t max);

    // ==============================================================================
    // series creation & management
    // ==============================================================================
    lv_chart_series_t* addSeries(lv_color_t color, lv_chart_axis_t axis);
    lv_chart_series_t* addSeries(lv_color_t color); // convenience overload (default axis)

    void removeSeries(lv_chart_series_t* series);
    void hideSeries(lv_chart_series_t* series, bool hide);

    void setSeriesColor(lv_chart_series_t* series, lv_color_t color);
    lv_color_t getSeriesColor(const lv_chart_series_t* series) const;

    lv_chart_series_t* getSeriesNext(const lv_chart_series_t* series) const;

    void setXStartPoint(lv_chart_series_t* series, uint32_t id);
    uint32_t getXStartPoint(lv_chart_series_t* series) const;

    // ==============================================================================
    // series value management
    // ==============================================================================
    void setAllValues(lv_chart_series_t* series, int32_t value);

    void setNextValue(lv_chart_series_t* series, int32_t value);
    void setNextValue2(lv_chart_series_t* series, int32_t xValue, int32_t yValue);

    void setSeriesValues(lv_chart_series_t* series, const int32_t values[], size_t count);
    void setSeriesValues2(lv_chart_series_t* series, const int32_t xValues[], const int32_t yValues[], size_t count);

    void setSeriesValueById(lv_chart_series_t* series, uint32_t id, int32_t value);
    void setSeriesValueById2(lv_chart_series_t* series, uint32_t id, int32_t xValue, int32_t yValue);

    void setSeriesExtYArray(lv_chart_series_t* series, int32_t array[]);
    void setSeriesExtXArray(lv_chart_series_t* series, int32_t array[]);

    int32_t* getSeriesYArray(lv_chart_series_t* series) const;
    int32_t* getSeriesXArray(lv_chart_series_t* series) const;

    // ==============================================================================
    // cursor management
    // ==============================================================================
    lv_chart_cursor_t* addCursor(lv_color_t color, lv_dir_t dir);
    void removeCursor(lv_chart_cursor_t* cursor);

    void setCursorPos(lv_chart_cursor_t* cursor, lv_point_t* pos);
    void setCursorPosX(lv_chart_cursor_t* cursor, int32_t x);
    void setCursorPosY(lv_chart_cursor_t* cursor, int32_t y);
    void setCursorPoint(lv_chart_cursor_t* cursor, lv_chart_series_t* series, uint32_t pointId);

    lv_point_t getCursorPoint(lv_chart_cursor_t* cursor) const;

    // ==============================================================================
    // point access & geometry
    // ==============================================================================
    void getPointPosById(lv_chart_series_t* series, uint32_t id, lv_point_t& out) const;

    uint32_t getPressedPoint() const;
    int32_t getFirstPointCenterOffset() const;

    // ==============================================================================
    // refresh
    // ==============================================================================
    void refresh();

};

} // namespace Garbox
