#pragma once

#include "app/types/EventData.h"
#include "app/types/EventType.h"
#include "app/types/ControllerId.h"
#include "core/controller/ControllerIfc.h"
#include "core/event/EventFactory.h"
#include "core/event/Event.h"

namespace Garbox {


class ControllerAbs : public ControllerIfc {
public:

    ControllerAbs(ControllerId controllerId);
    ~ControllerAbs();

    // interface implementations
    void setup(EventFactory& factory, EventForwarder& forwarder) final;
    void init() final;
    void start() final;
    void tick() final;
    ControllerId getControllerId() final;
    EventFactory& getEventFactory() final;

    // Disallow copy and move 
    ControllerAbs(const ControllerAbs&) = delete;
    ControllerAbs& operator=(const ControllerAbs&) = delete;
    ControllerAbs(ControllerAbs&&) = delete;
    ControllerAbs& operator=(ControllerAbs&&) = delete;

protected:

    // interface implementations
    virtual void sendEvent(Event* event) final;

    // abstract methods for user of class
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onTick() = 0;

private:

    ControllerId mControllerId = ControllerId::Null;
    EventFactory* mEventFactory = nullptr;
    EventForwarder* mEventForwarder = nullptr;

};

} // namespace
