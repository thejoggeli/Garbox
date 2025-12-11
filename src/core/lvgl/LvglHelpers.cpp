#include "LvglHelpers.h"
#include "core/assert/Assert.h"
#include "core/lvgl/BufferRotate.h"
#include <cstring>

namespace Garbox {

LvImage LvglHelpers::createRotatedLabelRgb565(LvObject& parent, const RotatedLabelConfig& config){

    // measure text size
    lv_point_t textSize;
    lv_text_get_size(
        &textSize, 
        config.text, 
        config.font,
        config.letterSpace,
        config.lineSpace,
        LV_COORD_MAX, 
        LV_TEXT_FLAG_NONE
    );

    AssertExit(textSize.x > 0, "LvglHelpers", "x must be >= 0");
    AssertExit(textSize.y > 0, "LvglHelpers", "y must be >= 0");

    const int32_t w = textSize.x;
    const int32_t h = textSize.y;

    // compute sizes for rotated pixels
    Angle angle = config.angle;
    uint32_t rotW = (angle == Angle::Deg90 || angle == Angle::Deg270) ? h : w;
    uint32_t rotH = (angle == Angle::Deg90 || angle == Angle::Deg270) ? w : h;

    // create buffer and canvas for rotated pixels first 
    // this minimizes fragmentation because if we created non-rotated buffer first
    // it would leave a "hole" in the memory after its free'd
    lv_draw_buf_t* drawBufRot = lv_draw_buf_create(rotW, rotH, LV_COLOR_FORMAT_RGB565, 0);
    AssertExit(drawBufRot != nullptr, "LvglHelpers", "draw buf create failed");
    
    // create rotated canvas
    lv_obj_t* rotCanvas = lv_canvas_create(parent.raw());
    lv_canvas_set_draw_buf(rotCanvas, drawBufRot);

    // create draw buffer (memory will be freed after this function)
    lv_draw_buf_t* drawBuf = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_RGB565, 0);
    AssertExit(drawBuf != nullptr, "LvglHelpers", "buffer allocation failed");

    // create canvas with this draw buffer (memory will be freed after this function)
    lv_obj_t* canvas = lv_canvas_create(lv_screen_active());
    lv_canvas_set_draw_buf(canvas, drawBuf);
    lv_canvas_fill_bg(canvas, lv_color_hex(config.bgColor), LV_OPA_COVER);

    // init layer
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    // draw the un-rotated label onto the canvas
    lv_draw_label_dsc_t dsc {};
    lv_draw_label_dsc_init(&dsc);
    dsc.text = config.text;
    dsc.color = lv_color_hex(config.color);
    dsc.font = config.font;
    dsc.letter_space = config.letterSpace;
    dsc.line_space = config.lineSpace;
    lv_area_t coords = {0, 0, w - 1, h - 1};
    lv_draw_label(&layer, &dsc, &coords);

    // finish layer
    lv_canvas_finish_layer(canvas, &layer);

    // copy pixels to rotated buffer
    uint8_t* srcBuf = (uint8_t*)drawBuf->data;
    uint8_t* dstBuf = (uint8_t*)drawBufRot->data;
    if(angle == Angle::Deg90){
        BufferRotate::rotate90(srcBuf, w, h, dstBuf, LV_COLOR_FORMAT_RGB565);
    }
    else if(angle == Angle::Deg180){
        BufferRotate::rotate180(srcBuf, w, h, dstBuf, LV_COLOR_FORMAT_RGB565);
    } 
    else if (angle == Angle::Deg270){
        BufferRotate::rotate270(srcBuf, w, h, dstBuf, LV_COLOR_FORMAT_RGB565);
    }

    // cleanup temporary allocated stuff
    lv_obj_delete(canvas); // free un-rotated canvas
    lv_draw_buf_destroy(drawBuf); // free un-rotated buffer

    return LvImage(rotCanvas);
}

} // namespace Garbox
