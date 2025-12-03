#pragma once

#include <cstddef>
#include <cstdint>
#include "core/lvgl/LvObject.h"

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;

namespace Garbox {

class LvglContext {
public:
    
    LvglContext(lv_obj_t* root, uint32_t displayWidth, uint32_t displayHeight);

    void init();

    LvObject& root();

    void getDisplaySize(uint32_t& width, uint32_t& height) const;

private:

    LvObject mRoot;
    uint32_t mDisplayWidth;
    uint32_t mDisplayHeight;

};

} // namespace Garbox
