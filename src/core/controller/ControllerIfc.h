#pragma once

namespace Garbox {

class EventSystem;

class ControllerIfc {

    virtual void setup(EventSystem& eventSystem) = 0;
    virtual void tick() = 0;

};

} // namespace
