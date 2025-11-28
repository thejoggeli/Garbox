#pragma once

#include "core/application/component/ComponentDescriptor.h"
#include "core/application/component/ComponentHostIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventView.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventAlias.h"
#include "shared/types/EventPayload.h"
#include "shared/types/EventType.h"

namespace Garbox {

class ComponentAbs {
public:

    ComponentAbs(ComponentType type, ComponentId id);
    ~ComponentAbs();

    // setup methhods
    void init(ComponentHostIfc& host);
    void start();

    // check if behaviour is initialized
    bool isInitialized() const { return mInitialized; }

    // get runtime context
    const RuntimeContext* getContext() { return mContext; }

    // get the host of this behaviour
    ComponentHostIfc* getHost() { return mHost; }

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

    // the descriptor of this component
    const ComponentDescriptor mComponentDescriptor;

    // abstract methods for deriving class
    virtual void onInit() = 0;
    virtual void onStart() = 0;

    // send event
    void publishEvent(const EventHeader* header);

    // make event
    template<EventType E>
    EventView<E> makeEvent(){
        if(!mInitialized){
            TriggerExit("BehaviourAbs", "not initialized");
        }
        EventView<E> event = mEventFactory->make<E>(mComponentDescriptor);
        if(!event.header()){
            TriggerExit("BehaviourAbs", "could not allocate event");
        }
        return event;
    }

private:

    // initialized flag
    bool mInitialized = false;
    
    // runtime context (injected in init() through host)
    const RuntimeContext* mContext = nullptr;

    // host of this component
    ComponentHostIfc* mHost = nullptr;

    // event factory (injected in init() through host)
    EventFactory* mEventFactory = nullptr;

};

} // namespace