#pragma once

#include <cstdint>
#include <lvgl.h>

#include "core/lvgl/objects/LvObject.h"
#include "core/util/container/heap/ArrayHeap.h"

namespace Garbox {

class ChartLabelRendererY {
public:
    ChartLabelRendererY(
        LvObject& renderTarget,
        uint32_t tickCount,
        lv_color_t textColor,
        lv_color_t backgroundColor,
        lv_opa_t backgroundOpa
    );

    void setTick(uint32_t index, int32_t permille, const char* text);

private:
    static void drawEventTrampoline(lv_event_t* event);
    void onDraw(lv_event_t* event);

    lv_coord_t permilleToPixelY(int32_t permille) const;

private:
    LvObject& mRenderTarget;

    ArrayHeap<int32_t> mTickPermilles;
    ArrayHeap<const char*> mTickTexts;

    lv_color_t mTextColor;
    lv_color_t mBackgroundColor;
    lv_opa_t   mBackgroundOpa;
};

} // namespace Garbox
