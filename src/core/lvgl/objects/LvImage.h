#pragma once

#include <lvgl.h>

#include "LvObject.h"

namespace Garbox {

class LvImage : public LvObject {
public:
    explicit LvImage(LvObject& parent);

    void setSource(const void* src);
    void setPivot(lv_coord_t x, lv_coord_t y);
    void setScale(uint32_t zoom);
};

} // namespace Garbox
