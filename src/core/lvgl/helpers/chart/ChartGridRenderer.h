#pragma once

#include <cstdint>
#include <lvgl.h>

#include "core/lvgl/objects/LvChart.h"
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

    void setXTickPosition(uint32_t index, float relativePosition);
    void setYTickPosition(uint32_t index, float relativePosition);

    void setXTickLabel(uint32_t index, const char* text);
    void setYTickLabel(uint32_t index, const char* text);

private:
    static void drawMainBeginTrampoline(lv_event_t* event);
    static void drawPostBeginTrampoline(lv_event_t* event);
    void onDrawGrid(lv_event_t* event);
    void onDrawLabels(lv_event_t* event);
    
    void getLabelTextSize(lv_draw_label_dsc_t& labelDsc, lv_point_t& outSize);

private:

    // tick position is internally used as scaled int to prevent float arithmetic during redraw
    static constexpr int32_t RelativePositionMax = 1024*64; // power of two for fast multiply and divide

    LvChart& mChart;
    
    struct Label {
        const char* text = nullptr;
    };

    struct Tick {
        int32_t relativePosition = RelativePositionMax / 2;
        Label label {};
    };

    ArrayHeap<Tick> mXTicks;
    ArrayHeap<Tick> mYTicks;

    lv_color_t mLineColor;
    lv_coord_t mLineThicknessPixels;
};

} // namespace Garbox
