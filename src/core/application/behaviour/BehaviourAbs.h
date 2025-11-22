#pragma once 

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class BehaviourAbs : public BehaviourIfc {
public:

    BehaviourAbs(ComponentId id);
    ~BehaviourAbs();

    // interface implementations
    void init(BehaviourHostIfc& host) final;
    void start() final;
    void setActive(bool active) final;
    bool isActive() const final;
    bool isInitialized() const final;
    ComponentId getComponentId() const final;
    const RuntimeContext* getContext() const final;
    BehaviourHostIfc* getHost() final;

    // disallow copy and move 
    BehaviourAbs(const BehaviourAbs&) = delete;
    BehaviourAbs& operator=(const BehaviourAbs&) = delete;
    BehaviourAbs(BehaviourAbs&&) = delete;
    BehaviourAbs& operator=(BehaviourAbs&&) = delete;

protected:

    // interface implementations
    void sendEvent(EventHeader* header) final;

    // make event
    template<typename EventPayload>
    EventWrite<EventPayload> makeEvent(){
        if(!mInitialized){
            TriggerExit("BehaviourAbs", "not initialized");
        }
        EventWrite<EventPayload> event = mEventFactory->make<EventPayload>(mComponentDescriptor);
        if(!event.header){
            TriggerExit("BehaviourAbs", "could not allocate event");
        }
        return event;
    }

    // abstract methods for user of class
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onBecomeActive() = 0;
    virtual void onBecomeInactive() = 0;

private:

    ComponentDescriptor mComponentDescriptor;
    BehaviourHostIfc* mHost = nullptr;
    const RuntimeContext* mContext = nullptr;
    EventFactory* mEventFactory = nullptr;

    bool mInitialized = false;
    bool mActive = false;

};

} // namespace