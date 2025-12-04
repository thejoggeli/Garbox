#pragma once

#include <lvgl.h>
#include "LvObject.h"

namespace Garbox {

class LvBox : public LvObject {
public:
    explicit LvBox(LvObject& parent);
};

} // namespace Garbox
