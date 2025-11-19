#pragma once 

#include "core/application/behaviour/BehaviourIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventView.h"
#include "core/application/event/EventWrapper.h"
#include "shared/types/EventData.h"

namespace Garbox {

class BehaviourAbs : public BehaviourIfc {
public:

    BehaviourAbs(ComponentId id);
    ~BehaviourAbs();

    // interface implementations
    void init(EventFactory& factory, EventForwarder& forwarder) final;
    void start() final;
    void setActive(bool active) final;
    bool isActive() const final;
    bool isInitialized() const final;
    ComponentId getComponentId() const final;

    // disallow copy and move 
    BehaviourAbs(const BehaviourAbs&) = delete;
    BehaviourAbs& operator=(const BehaviourAbs&) = delete;
    BehaviourAbs(BehaviourAbs&&) = delete;
    BehaviourAbs& operator=(BehaviourAbs&&) = delete;

protected:

    // interface implementations
    void sendEvent(Event* event) final;

    // make event
    template<typename EventDataType>
    EventWrapper<EventDataType> makeEvent(){
        if(!mInitialized){
            TriggerExit("BehaviourAbs", "not initialized");
        }
        EventWrapper wrapper = mEventFactory->make<EventDataType>(mComponentDescriptor);
        if(!wrapper.event){
            TriggerExit("BehaviourAbs", "could not allocate event");
        }
        return wrapper;
    }

    // abstract methods for user of class
    virtual void onInit() = 0;
    virtual void onStart() = 0;
    virtual void onBecomeActive() = 0;
    virtual void onBecomeInactive() = 0;

private:

    bool mInitialized = false;
    ComponentDescriptor mComponentDescriptor;
    EventFactory* mEventFactory = nullptr;
    EventForwarder* mEventForwarder = nullptr;

    bool mActive = false;

};

} // namespace