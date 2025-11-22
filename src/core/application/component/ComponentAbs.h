#pragma once

#include "core/application/component/ComponentDescriptor.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/types/EventWrite.h"
#include "core/application/event/types/EventRead.h"
#include "core/application/host/ComponentHostIfc.h"

namespace Garbox {

class ComponentAbs {
public:

    ComponentAbs(ComponentType type, ComponentId id);
    ~ComponentAbs();

    // setup methhods
    void init(ComponentHostIfc& host);
    void start();

    // get the host of this behaviour
    ComponentHostIfc* getComponentHost() { return mHost; }

    // check if behaviour is initialized
    bool isInitialized() const { return mInitialized; }

    // get the runtime context 
    const RuntimeContext* getContext() const { return mContext; }

    // component descriptor
    ComponentDescriptor getComponentDescriptor() const { return mComponentDescriptor; }
    ComponentType getComponentType() const { return mComponentDescriptor.type; }
    ComponentId getComponentId() const { return mComponentDescriptor.id; }

    // disallow copy and move 
    ComponentAbs(const ComponentAbs&) = delete;
    ComponentAbs& operator=(const ComponentAbs&) = delete;
    ComponentAbs(ComponentAbs&&) = delete;
    ComponentAbs& operator=(ComponentAbs&&) = delete;

protected:

    // abstract methods for derivingy class
    virtual void onInit() = 0;
    virtual void onStart() = 0;

    // host of this component
    ComponentHostIfc* mHost = nullptr;

    // initialized flag
    bool mInitialized = false;

    // the descriptor of this component
    const ComponentDescriptor mComponentDescriptor;

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

private:
    
    // runtime context (referenced from host)
    const RuntimeContext* mContext = nullptr;

    // event factory (referenced from host)
    EventFactory* mEventFactory = nullptr;

};

} // namespace