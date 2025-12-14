#pragma once
#include <lvgl.h>

namespace Garbox {

class BufferRotate {
public:
    static void rotate90 (const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt);
    static void rotate180(const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt);
    static void rotate270(const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt);
};

}
