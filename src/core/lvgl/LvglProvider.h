#pragma once

#include <cstddef>
#include <cstdint>
#include "core/lvgl/objects/LvObject.h"

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;

namespace Garbox::LvglProvider {
    
void Init(uint32_t displayWidth, uint32_t displayHeight);
LvObject& Root();

void GetDisplaySize(uint32_t& width, uint32_t& height);
uint32_t GetDisplayWidth();
uint32_t GetDisplayHeight();

} // namespace Garbox
