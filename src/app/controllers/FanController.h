#pragma once

#include "app/types/EventData.h"
#include "core/controller/ControllerAbs.h"

namespace Garbox {

class FanController : public ControllerAbs {
public:
    
    FanController();

    void onFanEvent(const EventWrapper<FanEventData>& event);
    void onButtonEvent(const EventWrapper<ButtonEventData>& event);

private:

    void onInit() final;
    void onStart() final;
    void onTick() final;

};

} // namespace