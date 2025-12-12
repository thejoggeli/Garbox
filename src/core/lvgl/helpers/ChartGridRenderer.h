#pragma once

#include <cstdint>
#include <lvgl.h>

#include "core/lvgl/objects/LvChart.h"
#include "core/lvgl/objects/LvObject.h"
#include "core/util/container/heap/ArrayHeap.h"

namespace Garbox {

class ChartGridRenderer {
public:
    ChartGridRenderer(
        LvChart& chart,
        uint32_t horizontalLineCount,
        uint32_t verticalLineCount,
        lv_color_t lineColor,
        lv_coord_t lineThicknessPixels
    );

    // set horizontal line position 
    // left:  permille = 0 
    // right: permille = 1000
    void setHorizontalPosition(uint32_t index, int32_t permille);

    // set vertical line position 
    // top:    permille = 0 
    // bottom: permille = 1000
    void setVerticalPosition(uint32_t index, int32_t permille); 

private:

    void updateHorizontalPosition(uint32_t index);
    void updateVerticalPosition(uint32_t index);
    
    void updateAllPositions();

    lv_coord_t permilleToPositionX(int32_t permille) const;
    lv_coord_t permilleToPositionY(int32_t permille) const;

private:

    LvChart& mChart;

    // owned objects
    ArrayHeap<LvObject> mHorizontalLines;
    ArrayHeap<LvObject> mVerticalLines;

    // stored values
    ArrayHeap<int32_t> mHorizontalPermilles;
    ArrayHeap<int32_t> mVerticalPermilles;

    // configuration
    lv_color_t mLineColor;
    lv_coord_t mLineThicknessPixels;

    // resize callbacks
    static void chartEventTrampoline(lv_event_t* event);
    void onChartEvent(lv_event_t* event);

};

} // namespace Garbox
