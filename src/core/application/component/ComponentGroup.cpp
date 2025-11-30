#include "ComponentGroup.h"

#include "core/log/Log.h"

namespace Garbox {

ComponentGroup::ComponentGroup(){
    // nothing to do 
}

void ComponentGroup::init(std::initializer_list<ComponentAbs*> components){
    AssertExit(!mInitialized, "ComponentGroup", "already initialized");

    const bool defaultEnabled = false;
    mComponents = new ComponentsArray(components.size());
    mTicksMatrix = new TicksMatrix(components.size(), defaultEnabled);
    mEventsMatrix = new EventsMatrix(components.size(), defaultEnabled);

    // store components pointers
    size_t index = 0;
    for(ComponentAbs* component : components) {
        component->setComponentGroup(this, index);
        mComponents->at(index) = component;
        index++;
    }

    mInitialized = true;
}

Span<ComponentAbs*> ComponentGroup::getComponents() { 
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    return Span(mComponents->data(), mComponents->size()); 
}

void ComponentGroup::setTicksEnabled(size_t componentIndex, bool enabled){
    // enable or disable all ticks this component should receive
    for (size_t tickIndex = 0; tickIndex < static_cast<size_t>(TickPhase::Count); tickIndex++){
        if(mTicksMatrix->at(tickIndex)[componentIndex]){
            mTicksMatrix->at(tickIndex).setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setEventsEnabled(size_t componentIndex, bool enabled){
    // enable or disable all events this component should receive
    for (size_t eventIndex = 0; eventIndex < static_cast<size_t>(EventType::Count); eventIndex++){
        if(mEventsMatrix->at(eventIndex)[componentIndex]){
            mEventsMatrix->at(eventIndex).setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setComponentEnabled(ComponentAbs* component, bool enabled) {
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t index = component->getComponentGroupIndex(); 
    if(mComponents->at(index)->isEnabled() == enabled){
        return;
    }
    mComponents->at(index)->setEnabled(enabled);
    setTicksEnabled(index, enabled);
    setEventsEnabled(index, enabled);
}

void ComponentGroup::enableAllComponents() {
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    for(ComponentAbs* component : *mComponents) {
        setComponentEnabled(component, true);
    }
}

void ComponentGroup::disableAllComponents() {
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    for(ComponentAbs* component : *mComponents) {
        setComponentEnabled(component, false);
    }
}

void ComponentGroup::setRouteTick(ComponentAbs* component, TickPhase phase, bool receive){
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t componentIndex = component->getComponentGroupIndex();
    const size_t tickIndex = static_cast<size_t>(phase); 
    mTicksMatrix->at(tickIndex)[componentIndex] = receive;
    if(mComponents->at(componentIndex)->isEnabled()){
        mTicksMatrix->at(tickIndex).setIndexEnabled(componentIndex, receive);
    }
}

void ComponentGroup::setRouteEvent(ComponentAbs* component, EventType event, bool receive){
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t componentIndex = component->getComponentGroupIndex();
    const size_t eventIndex = static_cast<size_t>(event);
    mEventsMatrix->at(eventIndex)[componentIndex] = receive;
    if(mComponents->at(componentIndex)->isEnabled()){
        mEventsMatrix->at(eventIndex).setIndexEnabled(componentIndex, receive);
    }
}

void ComponentGroup::routeTick(TickPhase phase) {
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    uint32_t count = 0;
    const size_t tickIndex = static_cast<size_t>(phase);
    for (auto it = mTicksMatrix->at(tickIndex).begin(); it != mTicksMatrix->at(tickIndex).end(); ++it) {
        mComponents->at(it.index())->receiveTick(phase);
        count++;
    }
    LogDebug("ticks", "%u, %s", count, TickPhaseToString(phase));
}

void ComponentGroup::routeEvent(const EventHeader* event) {
    AssertExit(mInitialized, "ComponentGroup", "not initialized");
    uint32_t count = 0;
    const size_t eventIndex = static_cast<size_t>(event->type);
    for (auto it = mEventsMatrix->at(eventIndex).begin(); it != mEventsMatrix->at(eventIndex).end(); ++it) {
        mComponents->at(it.index())->receiveEvent(event);
        count++;
    }
    LogDebug("events", "%u %s", count, EventTypeToString(event->type));
}

} // namespace Garbox
