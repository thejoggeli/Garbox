#pragma once

#include "core/controller/ControllerAbs.h"
#include "core/event/EventView.h"

namespace Garbox {

class Display;

class DisplayController : public ControllerAbs {
public:
    
    DisplayController();

    void onDisplayTick();

private:

    Display& mDisplay;

    void onInit() final;
    void onStart() final;
    void onTick() final;

};

} // namespace