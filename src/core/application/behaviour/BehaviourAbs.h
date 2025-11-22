#pragma once 

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/BehaviourHostIfc.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventPayload.h"

namespace Garbox {

class BehaviourAbs : public ComponentAbs, public BehaviourIfc {
public:

    BehaviourAbs(ComponentId componentId, BehaviourId behaviourId);
    ~BehaviourAbs();

    // interface implementations
    void init(BehaviourHostIfc& host) final;
    void start() final;
    void setActive(bool active) final;

    // trivial getters
    bool isInitialized() const { return mInitialized; }
    const RuntimeContext* getContext() const { return mContext; }
    BehaviourHostIfc* getHost() final { return mHost; }
    bool isActive() const { return mActive; }
    BehaviourId getControllerId() const { return mBehaviourId; }

    // disallow copy and move 
    BehaviourAbs(const BehaviourAbs&) = delete;
    BehaviourAbs& operator=(const BehaviourAbs&) = delete;
    BehaviourAbs(BehaviourAbs&&) = delete;
    BehaviourAbs& operator=(BehaviourAbs&&) = delete;

protected:

    // send event
    void sendEvent(EventHeader* header);

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

    BehaviourId mBehaviourId;
    BehaviourHostIfc* mHost = nullptr;
    const RuntimeContext* mContext = nullptr;
    EventFactory* mEventFactory = nullptr;

    bool mInitialized = false;
    bool mActive = false;

};

} // namespace