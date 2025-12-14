#include "BufferRotate.h"
#include <cstring>

namespace Garbox {

static inline uint32_t bpp(lv_color_format_t fmt) {
    return lv_color_format_get_size(fmt);
}

void BufferRotate::rotate90(const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt){
    const uint32_t pixelSize = bpp(fmt);
    for(uint32_t y = 0; y < h; y++) {
        for(uint32_t x = 0; x < w; x++) {
            uint32_t dx = h - 1 - y;
            uint32_t dy = x;

            const uint8_t* s = src + (y * w + x) * pixelSize;
            uint8_t* d       = dst + (dy * h + dx) * pixelSize;

            memcpy(d, s, pixelSize);
        }
    }
}

void BufferRotate::rotate180(const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt){
    const uint32_t pixelSize = bpp(fmt);
    for(uint32_t y = 0; y < h; y++) {
        for(uint32_t x = 0; x < w; x++) {
            uint32_t dx = w - 1 - x;
            uint32_t dy = h - 1 - y;

            const uint8_t* s = src + (y * w + x) * pixelSize;
            uint8_t* d       = dst + (dy * w + dx) * pixelSize;

            memcpy(d, s, pixelSize);
        }
    }
}

void BufferRotate::rotate270(const uint8_t* src, uint32_t w, uint32_t h, uint8_t* dst, lv_color_format_t fmt){
    const uint32_t pixelSize = bpp(fmt);
    for(uint32_t y = 0; y < h; y++) {
        for(uint32_t x = 0; x < w; x++) {
            uint32_t dx = y;
            uint32_t dy = w - 1 - x;

            const uint8_t* s = src + (y * w + x) * pixelSize;
            uint8_t* d       = dst + (dy * h + dx) * pixelSize;

            memcpy(d, s, pixelSize);
        }
    }
}

} // namespace Garbox
