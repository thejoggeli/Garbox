#pragma once

#include <cstddef>
#include <cstdint>
#include "core/lvgl/LvObject.h"

struct _lv_obj_t;
typedef struct _lv_obj_t lv_obj_t;

namespace Garbox {

class LvglContext {
public:
    
    LvglContext(lv_obj_t* root);

    void init();

    LvObject& getRoot();

    void setBoxPosition(float position);
    void setBackgroundColor(uint32_t color);

private:

    LvObject mRoot = nullptr;

    lv_obj_t* mBackground;
    lv_obj_t* mBox;

    void setFormatted(lv_obj_t* lbl, const char* fmt, ...);

};

} // namespace Garbox
