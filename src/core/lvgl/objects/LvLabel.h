#pragma once

#include <lvgl.h>

#include "LvObject.h"
#include "core/time/Time.h"

namespace Garbox {

class LvLabel : public LvObject {
public:

    LvLabel(LvObject& parent);

    void setText(const char* text);
    void setTextFormatted(const char* fmt, ...);
    void setLongMode(lv_label_long_mode_t mode);
    
};

} // namespace Garbox
