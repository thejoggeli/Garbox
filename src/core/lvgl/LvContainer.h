#pragma once

#include <lvgl.h>
#include "core/time/Time.h"
#include "LvObject.h"

namespace Garbox {

class LvContainer : public LvObject {
public:
    LvContainer(LvObject& parent);
};

} // namespace Garbox
