#include "ChartGridRenderer.h"

#include <algorithm>
#include "core/assert/Assert.h"

namespace Garbox {

ChartGridRenderer::ChartGridRenderer(
    // parameters
    LvChart& chart,
    uint32_t horizontalLineCount,
    uint32_t verticalLineCount,
    lv_color_t lineColor,
    lv_coord_t lineThicknessPixels):
    // initialize members
    mChart(chart),
    mHorizontalLines(horizontalLineCount, chart),
    mVerticalLines(verticalLineCount, chart),
    mHorizontalPermilles(horizontalLineCount),
    mVerticalPermilles(verticalLineCount),
    mLineColor(lineColor),
    mLineThicknessPixels(lineThicknessPixels){

    // init horizontal lines
    for(LvObject& line : mHorizontalLines){
        line.setScrollable(false);
        line.setBgColor(mLineColor);
        line.setBgOpa(LV_OPA_COVER);
    }

    // init vertical lines
    for(LvObject& line : mVerticalLines){
        line.setScrollable(false);
        line.setBgColor(mLineColor);
        line.setBgOpa(LV_OPA_COVER);
    }

    // resize callbacks
    mChart.addEventCallback(chartEventTrampoline, LV_EVENT_SIZE_CHANGED, this);
    mChart.addEventCallback(chartEventTrampoline, LV_EVENT_STYLE_CHANGED, this);
}

void ChartGridRenderer::setHorizontalPosition(uint32_t index, int32_t permille){
    if(index >= mHorizontalPermilles.size()){
        TriggerDebug("ChartGridRenderer", "horizontal index out of range");
        return;
    }

    mHorizontalPermilles[index] = permille;
    updateHorizontalPosition(index);
}

void ChartGridRenderer::setVerticalPosition(uint32_t index, int32_t permille){
    if(index >= mVerticalPermilles.size()){
        TriggerDebug("ChartGridRenderer", "vertical index out of range");
        return;
    }

    mVerticalPermilles[index] = permille;
    updateVerticalPosition(index);
}

void ChartGridRenderer::updateHorizontalPosition(uint32_t index){
    LvObject& line = mHorizontalLines[index];
    const lv_coord_t yPixel = permilleToPositionY(mHorizontalPermilles[index]);

    lv_area_t contentCoords;
    mChart.getContentCoords(contentCoords);
    
    line.setSize(mChart.getContentWidth(), mLineThicknessPixels);
    line.setPosition(contentCoords.x1, yPixel - (mLineThicknessPixels / 2));
}

void ChartGridRenderer::updateVerticalPosition(uint32_t index){
    LvObject& line = mVerticalLines[index];
    const lv_coord_t xPixel = permilleToPositionX(mVerticalPermilles[index]);

    lv_area_t contentCoords;
    mChart.getContentCoords(contentCoords);
    
    line.setSize(mLineThicknessPixels, mChart.getContentHeight());
    line.setPosition(xPixel - (mLineThicknessPixels / 2), contentCoords.y1);
}

void ChartGridRenderer::updateAllPositions(){
    for(uint32_t i = 0; i < mHorizontalLines.size(); i++){
        updateHorizontalPosition(i);
    }
    for(uint32_t i = 0; i < mVerticalLines.size(); i++){
        updateVerticalPosition(i);
    }
}

lv_coord_t ChartGridRenderer::permilleToPositionY(int32_t permille) const {
    permille = std::clamp<int32_t>(permille, 0, 1000);
    const lv_coord_t heightPixels = mChart.getContentHeight();
    const lv_coord_t offsetPixels = (heightPixels * permille) / 1000;
    return heightPixels - offsetPixels;
}

lv_coord_t ChartGridRenderer::permilleToPositionX(int32_t permille) const {    
    permille = std::clamp<int32_t>(permille, 0, 1000);
    const lv_coord_t widthPixels = mChart.getContentWidth();
    const lv_coord_t offsetPixels = (widthPixels * permille) / 1000;
    return offsetPixels;
}

void ChartGridRenderer::chartEventTrampoline(lv_event_t* event){
    ChartGridRenderer* self = static_cast<ChartGridRenderer*>(lv_event_get_user_data(event));
    if(self != nullptr){
        self->onChartEvent(event);
    }
}

void ChartGridRenderer::onChartEvent(lv_event_t* event){
    const lv_event_code_t code = lv_event_get_code(event);
    if((code == LV_EVENT_SIZE_CHANGED) || (code == LV_EVENT_STYLE_CHANGED)){
        updateAllPositions();
    }
}

} // namespace Garbox
