#pragma once

#include "core/controller/ControllerIfc.h"

namespace Garbox {

class ControllerAbs : public ControllerIfc {
public:

    ControllerAbs();
    ~ControllerAbs();

    // interface implementations
    void setup(EventSystem& eventSystem) final;
    void tick() final;

    // abstract methods for user of class
    virtual void onTick() = 0;

    // Disallow copy and move 
    ControllerAbs(const ControllerAbs&) = delete;
    ControllerAbs& operator=(const ControllerAbs&) = delete;
    ControllerAbs(ControllerAbs&&) = delete;
    ControllerAbs& operator=(ControllerAbs&&) = delete;

protected:

    template<typename EventData>
    void sendEvent(const EventData& data);

private:

    EventSystem* mEventSystem = nullptr;
};

} // namespace
