#include "ChartGridRenderer.h"

#include <algorithm>
#include <cstring>
#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

// label text
static const lv_font_t* LabelFont  = &lv_font_montserrat_12;
static const lv_color_t LabelColor = lv_color_hex(0xFFFFFF);
static const lv_opa_t   LabelOpa   = LV_OPA_60;

// label padding
static const int32_t LabelPaddingLeft   = 4;
static const int32_t LabelPaddingBottom = 4;

// label background
static const lv_color_t LabelBgColor  = lv_color_hex(0x0);
static const lv_opa_t   LabelBgOpa    = LV_OPA_30;
static const int32_t    LabelBgRadius = 2;
static const int32_t    LabelBgPadX   = 1;
static const int32_t    LabelBgPadY   = 0;

ChartGridRenderer::ChartGridRenderer(
    // parameters
    LvChart& chart,
    uint32_t xTicksCount,
    uint32_t yTicksCount,
    lv_color_t lineColor,
    lv_coord_t lineThicknessPixels):
    // initialize members
    mChart(chart),
    mXTicks(xTicksCount),
    mYTicks(yTicksCount),
    mLineColor(lineColor),
    mLineThicknessPixels(lineThicknessPixels){

    // register draw callback
    mChart.addEventCallback(drawMainBeginTrampoline, LV_EVENT_DRAW_MAIN_BEGIN, this);
    mChart.addEventCallback(drawPostBeginTrampoline, LV_EVENT_DRAW_POST_BEGIN, this);
}

void ChartGridRenderer::drawMainBeginTrampoline(lv_event_t* event){
    ChartGridRenderer* self = static_cast<ChartGridRenderer*>(lv_event_get_user_data(event));

    if(self != nullptr){
        self->onDrawGrid(event);
    }
}

void ChartGridRenderer::drawPostBeginTrampoline(lv_event_t* event){
    ChartGridRenderer* self = static_cast<ChartGridRenderer*>(lv_event_get_user_data(event));
    if(self != nullptr){
        self->onDrawLabels(event);
    }
}

void ChartGridRenderer::onDrawGrid(lv_event_t* event){
    lv_layer_t* layer = lv_event_get_layer(event);
    if(layer == nullptr){
        return;
    }

    lv_area_t contentArea;
    mChart.getContentCoords(contentArea);
    int32_t contentWidth = lv_area_get_width(&contentArea);
    int32_t contentHeight = lv_area_get_height(&contentArea);

    lv_draw_line_dsc_t line;
    lv_draw_line_dsc_init(&line);
    line.color = mLineColor;
    line.width = mLineThicknessPixels;
    line.opa   = LV_OPA_COVER;

    // render lines for x ticks (vertical lines)
    for(uint32_t i = 0; i < mXTicks.size(); i++){
        const int32_t x = contentArea.x1 + (contentWidth * mXTicks[i].relativePosition) / RelativePositionMax;
        line.p1 = { x, contentArea.y1 };
        line.p2 = { x, contentArea.y2 };
        lv_draw_line(layer, &line);
    }

    // render lines for y ticks (horizontal lines)
    for(uint32_t i = 0; i < mYTicks.size(); i++){
        const int32_t y = contentArea.y2 - (contentHeight * mYTicks[i].relativePosition) / RelativePositionMax;
        line.p1 = { contentArea.x1, y };
        line.p2 = { contentArea.x2, y };
        lv_draw_line(layer, &line);
    }
}

void ChartGridRenderer::onDrawLabels(lv_event_t* event){
    lv_layer_t* layer = lv_event_get_layer(event);
    if(layer == nullptr){
        return;
    }

    lv_area_t contentArea;
    mChart.getContentCoords(contentArea);
    int32_t contentWidth = lv_area_get_width(&contentArea);
    int32_t contentHeight = lv_area_get_height(&contentArea);

    lv_draw_label_dsc_t labelDsc;
    lv_draw_label_dsc_init(&labelDsc);
    labelDsc.color = LabelColor;
    labelDsc.opa = LabelOpa;
    labelDsc.font = LabelFont;

    lv_draw_rect_dsc_t rectDsc;
    lv_draw_rect_dsc_init(&rectDsc);
    rectDsc.bg_color = LabelBgColor;
    rectDsc.bg_opa   = LabelBgOpa;
    rectDsc.radius   = LabelBgRadius;

    // render labels for x ticks (bottom side)
    labelDsc.align = LV_TEXT_ALIGN_CENTER;
    for(uint32_t i = 0; i < mXTicks.size(); i++){
        Tick& tick = mXTicks[i];
        if(tick.label.text == nullptr){
            continue;
        }
        labelDsc.text = tick.label.text;

        // get pixel size of rendered text 
        getLabelTextSize(labelDsc, labelDsc.text_size);
        
        // compute coordinate of label
        const int32_t x1 = contentArea.x1 + ((contentWidth * mXTicks[i].relativePosition) / RelativePositionMax) - (labelDsc.text_size.x / 2);
        const int32_t x2 = x1 + labelDsc.text_size.x;
        const int32_t y2 = contentArea.y2 - LabelPaddingBottom;
        const int32_t y1 = y2 - labelDsc.text_size.y;

        // compute label area
        lv_area_t labelArea = {x1, y1, x2, y2};

        // compute bg rect area
        lv_area_t rectArea = {x1-LabelBgPadX, y1-LabelBgPadY, x2+LabelBgPadX, y2+LabelBgPadY};

        // draw 
        // lv_draw_rect(layer, &rectDsc, &rectArea);
        lv_draw_label(layer, &labelDsc, &labelArea);
    }

    // render lines for y ticks (left side)
    labelDsc.align = LV_TEXT_ALIGN_LEFT;
    for(uint32_t i = 0; i < mYTicks.size(); i++){
        Tick& tick = mYTicks[i];
        if(tick.label.text == nullptr){
            continue;
        }
        labelDsc.text = tick.label.text;

        // get pixel size of rendered text 
        getLabelTextSize(labelDsc, labelDsc.text_size);
        
        // compute coordinate of label
        const int32_t x1 = contentArea.x1 + LabelPaddingLeft; 
        const int32_t y1 = contentArea.y2 - ((contentHeight * mYTicks[i].relativePosition) / RelativePositionMax) - (labelDsc.text_size.y / 2);
        const int32_t x2 = x1 + labelDsc.text_size.x; 
        const int32_t y2 = y1 + labelDsc.text_size.y;

        // compute label area
        lv_area_t labelArea = {x1, y1, x2, y2};

        // compute bg rect area
        lv_area_t rectArea = {x1-LabelBgPadX, y1-LabelBgPadY, x2+LabelBgPadX, y2+LabelBgPadY};

        // draw 
        // lv_draw_rect(layer, &rectDsc, &rectArea);
        lv_draw_label(layer, &labelDsc, &labelArea);
    }
}

void ChartGridRenderer::getLabelTextSize(lv_draw_label_dsc_t& labelDsc, lv_point_t& outSize){
    lv_txt_get_size(
        &outSize,
        labelDsc.text,
        labelDsc.font,
        labelDsc.letter_space,
        labelDsc.line_space,
        LV_COORD_MAX,
        LV_TEXT_FLAG_NONE
    );
}

void ChartGridRenderer::setXTickPosition(uint32_t index, float relativePosition){
    AssertExit(index < mXTicks.size(), "ChartGridRenderer", "index out of range");
    const int32_t scalePosition = static_cast<int32_t>(relativePosition * RelativePositionMax + 0.5f);
    mXTicks[index].relativePosition = std::clamp<int32_t>(scalePosition, 0, RelativePositionMax);
    lv_obj_invalidate(mChart.raw());
}

void ChartGridRenderer::setYTickPosition(uint32_t index, float relativePosition){
    AssertExit(index < mYTicks.size(), "ChartGridRenderer", "index out of range");
    const int32_t scalePosition = static_cast<int32_t>(relativePosition * RelativePositionMax + 0.5f);
    mYTicks[index].relativePosition = std::clamp<int32_t>(scalePosition, 0, RelativePositionMax);
    lv_obj_invalidate(mChart.raw());
}

void ChartGridRenderer::setXTickLabel(uint32_t index, const char* text){
    AssertExit(index < mXTicks.size(), "ChartGridRenderer", "index out of range");
    mXTicks[index].label.text = (std::strcmp(text, "") == 0) ? nullptr : text;
    lv_obj_invalidate(mChart.raw());
}

void ChartGridRenderer::setYTickLabel(uint32_t index, const char* text){
    AssertExit(index < mYTicks.size(), "ChartGridRenderer", "index out of range");
    mYTicks[index].label.text = (std::strcmp(text, "") == 0) ? nullptr : text;
    lv_obj_invalidate(mChart.raw());
}

} // namespace Garbox
