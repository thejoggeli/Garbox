#pragma once

#include "core/controller/ControllerAbs.h"
#include "global/events/EventData.h"

namespace Garbox {

class FanController : public ControllerAbs {

    void onTick() final;
    void handleButtonEvent(const ButtonEvent& event);

};

} // namespace