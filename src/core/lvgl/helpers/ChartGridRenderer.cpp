#include "ChartGridRenderer.h"

#include <algorithm>
#include "core/assert/Assert.h"

namespace Garbox {

ChartGridRenderer::ChartGridRenderer(
    // parameters
    LvObject& renderTarget,
    uint32_t horizontalLineCount,
    uint32_t verticalLineCount,
    lv_color_t lineColor,
    lv_coord_t lineThicknessPixels):
    // initialize members
    mRenderTarget(renderTarget),
    mHorizontalPermilles(horizontalLineCount),
    mVerticalPermilles(verticalLineCount),
    mLineColor(lineColor),
    mLineThicknessPixels(lineThicknessPixels){

    // configure overlay
    mRenderTarget.setSize(LV_PCT(100), LV_PCT(100));
    mRenderTarget.setAlign(LV_ALIGN_TOP_LEFT);
    mRenderTarget.setScrollable(false);
    mRenderTarget.setBgOpa(LV_OPA_TRANSP);

    // ensure it is drawn below the chart
    mRenderTarget.moveBackground();

    // register draw callback
    mRenderTarget.addEventCallback(drawEventTrampoline, LV_EVENT_DRAW_MAIN, this);
}

void ChartGridRenderer::drawEventTrampoline(lv_event_t* event){
    ChartGridRenderer* self = static_cast<ChartGridRenderer*>(lv_event_get_user_data(event));

    if(self != nullptr){
        self->onDraw(event);
    }
}

void ChartGridRenderer::onDraw(lv_event_t* event){
    lv_layer_t* layer = lv_event_get_layer(event);
    if(layer == nullptr){
        return;
    }

    lv_area_t contentArea;
    mRenderTarget.getContentCoords(contentArea);

    lv_draw_line_dsc_t lineDsc;
    lv_draw_line_dsc_init(&lineDsc);
    lineDsc.color = mLineColor;
    lineDsc.width = mLineThicknessPixels;
    lineDsc.opa   = LV_OPA_COVER;
    lineDsc.round_start = 1;
    lineDsc.round_end = 1;

    // horizontal lines
    for(uint32_t i = 0; i < mHorizontalPermilles.size(); i++){
        const lv_coord_t y = permilleToPixelY(mHorizontalPermilles[i]);

        lineDsc.p1 = { contentArea.x1, y };
        lineDsc.p2 = { contentArea.x2, y };

        lv_draw_line(layer, &lineDsc);
    }

    // vertical lines
    for(uint32_t i = 0; i < mVerticalPermilles.size(); i++){
        const lv_coord_t x = permilleToPixelX(mVerticalPermilles[i]);

        lineDsc.p1 = { x, contentArea.y1 };
        lineDsc.p2 = { x, contentArea.y2 };

        lv_draw_line(layer, &lineDsc);
    }
}

void ChartGridRenderer::setHorizontalPosition(uint32_t index, int32_t permille){
    if(index >= mHorizontalPermilles.size()){
        TriggerDebug("ChartGridRenderer", "horizontal index out of range");
        return;
    }

    mHorizontalPermilles[index] = permille;
    lv_obj_invalidate(mRenderTarget.raw());
}

void ChartGridRenderer::setVerticalPosition(uint32_t index, int32_t permille){
    if(index >= mVerticalPermilles.size()){
        TriggerDebug("ChartGridRenderer", "vertical index out of range");
        return;
    }

    mVerticalPermilles[index] = permille;
    lv_obj_invalidate(mRenderTarget.raw());
}

lv_coord_t ChartGridRenderer::permilleToPixelY(int32_t permille) const {
    permille = std::clamp<int32_t>(permille, 0, 1000);
    const lv_coord_t heightPixels = mRenderTarget.getContentHeight();
    const lv_coord_t offsetPixels = (heightPixels * permille) / 1000;
    return heightPixels - offsetPixels;
}

lv_coord_t ChartGridRenderer::permilleToPixelX(int32_t permille) const {    
    permille = std::clamp<int32_t>(permille, 0, 1000);
    const lv_coord_t widthPixels = mRenderTarget.getContentWidth();
    const lv_coord_t offsetPixels = (widthPixels * permille) / 1000;
    return offsetPixels;
}

} // namespace Garbox
