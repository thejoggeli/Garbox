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
        LvObject& renderTarget,
        uint32_t horizontalLineCount,
        uint32_t verticalLineCount,
        lv_color_t lineColor,
        lv_coord_t lineThicknessPixels
    );

    void setHorizontalPosition(uint32_t index, int32_t permille);
    void setVerticalPosition(uint32_t index, int32_t permille);

private:
    static void drawEventTrampoline(lv_event_t* event);
    void onDraw(lv_event_t* event);

    lv_coord_t permilleToPixelY(int32_t permille) const;
    lv_coord_t permilleToPixelX(int32_t permille) const;

private:
    LvObject& mRenderTarget;

    ArrayHeap<int32_t> mHorizontalPermilles;
    ArrayHeap<int32_t> mVerticalPermilles;

    lv_color_t mLineColor;
    lv_coord_t mLineThicknessPixels;
};


} // namespace Garbox
