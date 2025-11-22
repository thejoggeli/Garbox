#pragma once

#include "core/application/component/ComponentAbs.h"
#include "core/application/controller/ControllerIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/ControllerHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class EventFactory;

class ControllerAbs : public ComponentAbs, public ControllerIfc {
public:

    ControllerAbs(ComponentId componentId, ControllerId controllerId);
    ~ControllerAbs();

    // interface implementations
    void init(ControllerHostIfc& host) final;
    void start() final;

    // trivial getters
    bool isInitialized() const { return mInitialized; }
    const RuntimeContext* getContext() const { return mContext; }
    ControllerHostIfc* getHost() final { return mHost; }
    ControllerId getControllerId() const { return mControllerId; }

    // disallow copy and move 
    ControllerAbs(const ControllerAbs&) = delete;
    ControllerAbs& operator=(const ControllerAbs&) = delete;
    ControllerAbs(ControllerAbs&&) = delete;
    ControllerAbs& operator=(ControllerAbs&&) = delete;

protected:

    // send event
    void sendEvent(EventHeader* header);

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

    ControllerId mControllerId;
    ControllerHostIfc* mHost = nullptr;
    const RuntimeContext* mContext = nullptr;
    EventFactory* mEventFactory = nullptr;
    
    bool mInitialized = false;

};

} // namespace
