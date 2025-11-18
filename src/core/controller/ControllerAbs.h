#pragma once

#include "core/controller/ControllerIfc.h"
#include "core/event/EventFactory.h"
#include "core/event/Event.h"
#include "shared/types/EventData.h"
#include "shared/types/EventType.h"

namespace Garbox {

class ControllerAbs : public ControllerIfc {
public:

    ControllerAbs(ControllerId controllerId);
    ~ControllerAbs();

    // interface implementations
    void init(EventFactory& factory, EventForwarder& forwarder) final;
    void start() final;
    ControllerId getControllerId() final;
    EventFactory& getEventFactory() final;

    // Disallow copy and move 
    ControllerAbs(const ControllerAbs&) = delete;
    ControllerAbs& operator=(const ControllerAbs&) = delete;
    ControllerAbs(ControllerAbs&&) = delete;
    ControllerAbs& operator=(ControllerAbs&&) = delete;

protected:

    // interface implementations
    void sendEvent(Event* event) final;

    // abstract methods for user of class
    virtual void onInit() = 0;
    virtual void onStart() = 0;

private:

    ControllerId mControllerId = ControllerId::Null;
    EventFactory* mEventFactory = nullptr;
    EventForwarder* mEventForwarder = nullptr;

};

} // namespace
