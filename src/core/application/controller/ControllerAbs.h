#pragma once

#include "core/application/controller/ControllerIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/ControllerHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class EventFactory;

class ControllerAbs : public ControllerIfc {
public:

    ControllerAbs(ComponentId id);
    ~ControllerAbs();

    // interface implementations
    void init(ControllerHostIfc& host) final;
    void start() final;
    bool isInitialized() const final;
    ComponentId getComponentId() const final;
    const RuntimeContext* getContext() const final;
    ControllerHostIfc* getHost() final;

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

    ComponentDescriptor mComponentDescriptor;
    ControllerHostIfc* mHost = nullptr;
    const RuntimeContext* mContext = nullptr;
    EventFactory* mEventFactory = nullptr;
    
    bool mInitialized = false;

};

} // namespace
