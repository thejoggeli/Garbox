#pragma once

#include "core/application/component/ComponentDescriptor.h"
#include "core/application/component/ComponentHostIfc.h"
#include "core/application/event/EventFactory.h"
#include "core/application/event/EventView.h"
#include "core/application/runtime/RuntimeContext.h"
#include "shared/types/EventAlias.h"
#include "shared/types/EventPayload.h"
#include "shared/types/EventType.h"
#include "shared/types/TickPhase.h"

namespace Garbox {

class RuntimeAbs;

class ComponentAbs {
public:

    ComponentAbs(ComponentType type, ComponentId id);
    ~ComponentAbs();

    // setup methhods
    virtual void init(ComponentHostIfc& host);
    virtual void start();

    // components can be enabled and disabled
    bool isEnabled() const { return mEnabled; }

    // check if behaviour is initialized
    bool isInitialized() const { return mInitialized; }

    // get runtime context
    const RuntimeContext* getContext() { return mContext; }

    // get the host of this behaviour
    ComponentHostIfc* host() { return mHost; }

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

    virtual void becomeEnabled();
    virtual void becomeDisabled();

    // abstract methods for user implementation
    virtual void onInit() {};
    virtual void onStart() {};
    virtual void onBecomeEnabled() {};  
    virtual void onBecomeDisabled() {}; 

    // send event
    void sendEventToHost(const EventHeader* header);

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

    bool mEnabled = false;
    bool mInitialized = false;

    // the descriptor of this component
    const ComponentDescriptor mComponentDescriptor;
    
    // runtime context (injected in init() through host)
    const RuntimeContext* mContext = nullptr;

    // host of this component
    ComponentHostIfc* mHost = nullptr;

    // event factory (injected in init() through host)
    EventFactory* mEventFactory = nullptr;

    // only the RuntimeAbs class is allowed to access setEnabled()
    void setEnabled(bool enabled);

    friend class RuntimeAbs;
};

} // namespace