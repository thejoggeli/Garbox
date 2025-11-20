#pragma once

#include "core/application/controller/ControllerIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "shared/types/EventData.h"

namespace Garbox {

class ControllerAbs : public ControllerIfc {
public:

    ControllerAbs(ComponentId id);
    ~ControllerAbs();

    // interface implementations
    void init(EventFactory& factory, EventForwarder& forwarder) final;
    void start() final;
    bool isInitialized() const final;
    ComponentId getComponentId() const final;

    // disallow copy and move 
    ControllerAbs(const ControllerAbs&) = delete;
    ControllerAbs& operator=(const ControllerAbs&) = delete;
    ControllerAbs(ControllerAbs&&) = delete;
    ControllerAbs& operator=(ControllerAbs&&) = delete;

protected:

    // interface implementations
    void sendEvent(EventHeader* header) final;

    // make event
    template<typename EventPayload>
    EventWrite<EventPayload> makeEvent(){
        if(!mInitialized){
            TriggerExit("ControllerAbs", "not initialized");
        }
        EventWrite<EventPayload> event = mEventFactory->make<EventPayload>(mComponentDescriptor);
        if(!event.header){
            TriggerExit("ControllerAbs", "could not allocate event");
        }
        return event;
    }

    // abstract methods for user of class
    virtual void onInit() = 0;
    virtual void onStart() = 0;

private:

    bool mInitialized = false;
    ComponentDescriptor mComponentDescriptor;
    EventFactory* mEventFactory = nullptr;
    EventForwarder* mEventForwarder = nullptr;

};

} // namespace
