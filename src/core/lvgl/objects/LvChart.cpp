#include "LvChart.h"

namespace Garbox {

// ==============================================================================
// construction
// ==============================================================================
LvChart::LvChart(LvObject& parent)
    : LvObject(lv_chart_create(parent.raw())) {}

LvChart::LvChart(lv_obj_t* raw)
    : LvObject(raw) {}

// ==============================================================================
// type & configuration
// ==============================================================================
void LvChart::setType(lv_chart_type_t type){
    lv_chart_set_type(mRaw, type);
}

void LvChart::setPointCount(uint32_t count){
    lv_chart_set_point_count(mRaw, count);
}

void LvChart::setUpdateMode(lv_chart_update_mode_t mode){
    lv_chart_set_update_mode(mRaw, mode);
}

void LvChart::setDivLineCount(uint32_t hDiv, uint32_t vDiv){
    lv_chart_set_div_line_count(mRaw, hDiv, vDiv);
}

void LvChart::setHorDivLineCount(uint32_t count){
    lv_chart_set_hor_div_line_count(mRaw, count);
}

void LvChart::setVerDivLineCount(uint32_t count){
    lv_chart_set_ver_div_line_count(mRaw, count);
}

lv_chart_type_t LvChart::getType() const {
    return lv_chart_get_type(mRaw);
}

uint32_t LvChart::getPointCount() const {
    return lv_chart_get_point_count(mRaw);
}


// ==============================================================================
// axis & ranges
// ==============================================================================
void LvChart::setAxisRange(lv_chart_axis_t axis, int32_t min, int32_t max){
    lv_chart_set_axis_range(mRaw, axis, min, max);
}

void LvChart::setAxisMinValue(lv_chart_axis_t axis, int32_t min){
    lv_chart_set_axis_min_value(mRaw, axis, min);
}

void LvChart::setAxisMaxValue(lv_chart_axis_t axis, int32_t max){
    lv_chart_set_axis_max_value(mRaw, axis, max);
}

// ==============================================================================
// series creation & management
// ==============================================================================
lv_chart_series_t* LvChart::addSeries(lv_color_t color, lv_chart_axis_t axis){
    return lv_chart_add_series(mRaw, color, axis);
}

lv_chart_series_t* LvChart::addSeries(lv_color_t color){
    return lv_chart_add_series(mRaw, color, LV_CHART_AXIS_PRIMARY_Y);
}

void LvChart::resetSeries(lv_chart_series_t* series){
    lv_chart_set_all_values(mRaw, series, LV_CHART_POINT_NONE);
}

void LvChart::removeSeries(lv_chart_series_t* series){
    lv_chart_remove_series(mRaw, series);
}

void LvChart::hideSeries(lv_chart_series_t* series, bool hide){
    lv_chart_hide_series(mRaw, series, hide);
}

void LvChart::setSeriesColor(lv_chart_series_t* series, lv_color_t color){
    lv_chart_set_series_color(mRaw, series, color);
}

lv_color_t LvChart::getSeriesColor(const lv_chart_series_t* series) const {
    return lv_chart_get_series_color(mRaw, series);
}

lv_chart_series_t* LvChart::getSeriesNext(const lv_chart_series_t* series) const {
    return lv_chart_get_series_next(mRaw, series);
}

void LvChart::setXStartPoint(lv_chart_series_t* series, uint32_t id){
    lv_chart_set_x_start_point(mRaw, series, id);
}

uint32_t LvChart::getXStartPoint(lv_chart_series_t* series) const {
    return lv_chart_get_x_start_point(mRaw, series);
}


// ==============================================================================
// series value management
// ==============================================================================
void LvChart::setAllValues(lv_chart_series_t* series, int32_t value){
    lv_chart_set_all_values(mRaw, series, value);
}

void LvChart::setNextValue(lv_chart_series_t* series, int32_t value){
    lv_chart_set_next_value(mRaw, series, value);
}

void LvChart::setNextValue2(lv_chart_series_t* series, int32_t xValue, int32_t yValue){
    lv_chart_set_next_value2(mRaw, series, xValue, yValue);
}

void LvChart::setSeriesValues(lv_chart_series_t* series, const int32_t values[], size_t count){
    lv_chart_set_series_values(mRaw, series, values, count);
}

void LvChart::setSeriesValues2(lv_chart_series_t* series, const int32_t xValues[], const int32_t yValues[], size_t count){
    lv_chart_set_series_values2(mRaw, series, xValues, yValues, count);
}

void LvChart::setSeriesValueById(lv_chart_series_t* series, uint32_t id, int32_t value){
    lv_chart_set_series_value_by_id(mRaw, series, id, value);
}

void LvChart::setSeriesValueById2(lv_chart_series_t* series, uint32_t id, int32_t xValue, int32_t yValue){
    lv_chart_set_series_value_by_id2(mRaw, series, id, xValue, yValue);
}

void LvChart::setSeriesExtYArray(lv_chart_series_t* series, int32_t array[]){
    lv_chart_set_series_ext_y_array(mRaw, series, array);
}

void LvChart::setSeriesExtXArray(lv_chart_series_t* series, int32_t array[]){
    lv_chart_set_series_ext_x_array(mRaw, series, array);
}

int32_t* LvChart::getSeriesYArray(lv_chart_series_t* series) const {
    return lv_chart_get_series_y_array(mRaw, series);
}

int32_t* LvChart::getSeriesXArray(lv_chart_series_t* series) const {
    return lv_chart_get_series_x_array(mRaw, series);
}


// ==============================================================================
// cursors
// ==============================================================================
lv_chart_cursor_t* LvChart::addCursor(lv_color_t color, lv_dir_t dir){
    return lv_chart_add_cursor(mRaw, color, dir);
}

void LvChart::removeCursor(lv_chart_cursor_t* cursor){
    lv_chart_remove_cursor(mRaw, cursor);
}

void LvChart::setCursorPos(lv_chart_cursor_t* cursor, lv_point_t* pos){
    lv_chart_set_cursor_pos(mRaw, cursor, pos);
}

void LvChart::setCursorPosX(lv_chart_cursor_t* cursor, int32_t x){
    lv_chart_set_cursor_pos_x(mRaw, cursor, x);
}

void LvChart::setCursorPosY(lv_chart_cursor_t* cursor, int32_t y){
    lv_chart_set_cursor_pos_y(mRaw, cursor, y);
}

void LvChart::setCursorPoint(lv_chart_cursor_t* cursor, lv_chart_series_t* series, uint32_t pointId){
    lv_chart_set_cursor_point(mRaw, cursor, series, pointId);
}

lv_point_t LvChart::getCursorPoint(lv_chart_cursor_t* cursor) const {
    return lv_chart_get_cursor_point(mRaw, cursor);
}


// ==============================================================================
// point access & geometry
// ==============================================================================
void LvChart::getPointPosById(lv_chart_series_t* series, uint32_t id, lv_point_t& out) const {
    lv_chart_get_point_pos_by_id(mRaw, series, id, &out);
}

uint32_t LvChart::getPressedPoint() const {
    return lv_chart_get_pressed_point(mRaw);
}

int32_t LvChart::getFirstPointCenterOffset() const {
    return lv_chart_get_first_point_center_offset(mRaw);
}


// ==============================================================================
// refresh
// ==============================================================================
void LvChart::refresh(){
    lv_chart_refresh(mRaw);
}

} // namespace Garbox
