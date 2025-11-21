#pragma once

#include "app/controllers/generated/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController(const RuntimeContext& context);

    void onRenderTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox