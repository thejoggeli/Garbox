#include "ChartLabelRendererY.h"

#include <algorithm>
#include "core/assert/Assert.h"

namespace Garbox {

ChartLabelRendererY::ChartLabelRendererY(
    // parameters
    LvObject& renderTarget,
    uint32_t tickCount,
    lv_color_t textColor,
    lv_color_t backgroundColor,
    lv_opa_t backgroundOpa):
    // initialize members
    mRenderTarget(renderTarget),
    mTickPermilles(tickCount),
    mTickTexts(tickCount),
    mTextColor(textColor),
    mBackgroundColor(backgroundColor),
    mBackgroundOpa(backgroundOpa){

    mRenderTarget.setSize(LV_PCT(100), LV_PCT(100));
    mRenderTarget.setAlign(LV_ALIGN_TOP_LEFT);
    mRenderTarget.setScrollable(false);
    mRenderTarget.setBgOpa(LV_OPA_TRANSP);

    // drawn on top of chart
    mRenderTarget.moveForeground();

    mRenderTarget.addEventCallback(
        drawEventTrampoline,
        LV_EVENT_DRAW_POST,
        this
    );
}

void ChartLabelRendererY::setTick(
    uint32_t index,
    int32_t permille,
    const char* text
){
    if(index >= mTickPermilles.size()){
        TriggerDebug("ChartLabelRendererY", "tick index out of range");
        return;
    }

    mTickPermilles[index] = permille;
    mTickTexts[index] = text;

    lv_obj_invalidate(mRenderTarget.raw());
}

void ChartLabelRendererY::drawEventTrampoline(lv_event_t* event){
    ChartLabelRendererY* self =
        static_cast<ChartLabelRendererY*>(lv_event_get_user_data(event));

    if(self != nullptr){
        self->onDraw(event);
    }
}

void ChartLabelRendererY::onDraw(lv_event_t* event){
    lv_layer_t* layer = lv_event_get_layer(event);
    if(layer == nullptr){
        return;
    }

    lv_area_t contentArea;
    mRenderTarget.getContentCoords(contentArea);

    lv_draw_label_dsc_t labelDsc;
    lv_draw_label_dsc_init(&labelDsc);
    labelDsc.color = mTextColor;
    labelDsc.outline_stroke_opa = LV_PCT(50);
    labelDsc.outline_stroke_width = 2;
    labelDsc.outline_stroke_color = lv_color_hex(0xFF00FF);

    lv_draw_rect_dsc_t rectDsc;
    lv_draw_rect_dsc_init(&rectDsc);
    rectDsc.bg_color = mBackgroundColor;
    rectDsc.bg_opa   = mBackgroundOpa;
    rectDsc.radius   = 2;

    for(uint32_t i = 0; i < mTickPermilles.size(); i++){
        const char* text = mTickTexts[i];
        if(text == nullptr){
            continue;
        }
        labelDsc.text = text;

        lv_point_t textSize;
        lv_txt_get_size(
            &textSize,
            text,
            labelDsc.font,
            labelDsc.letter_space,
            labelDsc.line_space,
            LV_COORD_MAX,
            LV_TEXT_FLAG_NONE
        );
        textSize.x += labelDsc.outline_stroke_width * 2;
        textSize.y += labelDsc.outline_stroke_width * 2;

        const lv_coord_t y = permilleToPixelY(mTickPermilles[i]) - (textSize.y / 2);

        const lv_area_t bgArea = {
            contentArea.x1,
            y - 1,
            contentArea.x1 + textSize.x + 4,
            y + textSize.y + 1
        };

        lv_draw_rect(layer, &rectDsc, &bgArea);

        lv_area_t labelArea = bgArea;
        labelArea.x1 += 2;

        lv_draw_label(layer, &labelDsc, &labelArea);
    }
}

lv_coord_t ChartLabelRendererY::permilleToPixelY(int32_t permille) const {
    permille = std::clamp<int32_t>(permille, 0, 1000);

    const lv_coord_t heightPixels = mRenderTarget.getContentHeight();
    const lv_coord_t offsetPixels = (heightPixels * permille) / 1000;

    return mRenderTarget.getContentHeight() - offsetPixels;
}

} // namespace Garbox
