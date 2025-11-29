#include "ComponentGroup.h"

namespace Garbox {

ComponentGroup::ComponentGroup(std::initializer_list<ComponentAbs*> components): 
    // init members    
    mComponents(components.size()),
    mTickMatrix(components.size(), false),  // initialize ArrayMaskHeap(elementCount, defaultEnabled)
    mEventMatrix(components.size(), false){ // initialize ArrayMaskHeap(elementCount, defaultEnabled)

    // store components pointers
    size_t index = 0;
    for(ComponentAbs* component : components) {
        component->setComponentGroup(this, index);
        mComponents[index] = component;
        index++;
    }
}

void ComponentGroup::setTicksEnabled(size_t componentIndex, bool enabled){
    // enable/disable all ticks this component should receive
    for (size_t tickIndex = 0; tickIndex < static_cast<size_t>(TickPhase::Count); tickIndex++){
        if(mTickMatrix[tickIndex][componentIndex]){
            mTickMatrix[tickIndex].setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setEventsEnabled(size_t componentIndex, bool enabled){
    // enable/disable all events this component should receive
    for (size_t eventIndex = 0; eventIndex < static_cast<size_t>(EventType::Count); eventIndex++){
        if(mEventMatrix[eventIndex][componentIndex]){
            mEventMatrix[eventIndex].setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setComponentEnabled(ComponentAbs* component, bool enabled) {
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t index = component->getComponentGroupIndex(); 
    if(mComponents[index]->isEnabled() == enabled){
        return;
    }
    mComponents[index]->setEnabled(enabled);
    setTicksEnabled(index, enabled);
    setEventsEnabled(index, enabled);
}

void ComponentGroup::enableAllComponents() {
    for(ComponentAbs* component : mComponents) {
        setComponentEnabled(component, true);
    }
}

void ComponentGroup::disableAllComponents() {
    for(ComponentAbs* component : mComponents) {
        setComponentEnabled(component, false);
    }
}

void ComponentGroup::setReceiveTick(ComponentAbs* component, TickPhase phase, bool receive){
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t componentIndex = component->getComponentGroupIndex();
    const size_t tickIndex = static_cast<size_t>(phase); 
    mTickMatrix[tickIndex][componentIndex] = receive;
    if(mComponents[componentIndex]->isEnabled()){
        mTickMatrix[tickIndex].setIndexEnabled(componentIndex, receive);
    }
}

void ComponentGroup::setReceiveEvent(ComponentAbs* component, EventType event, bool receive){
    AssertExit(component->getComponentGroup() == this, "ComponentGroup", "invalid component");
    const size_t componentIndex = component->getComponentGroupIndex();
    const size_t eventIndex = static_cast<size_t>(event);
    mEventMatrix[eventIndex][componentIndex] = receive;
    if(mComponents[componentIndex]->isEnabled()){
        mEventMatrix[eventIndex].setIndexEnabled(componentIndex, receive);
    }
}

void ComponentGroup::receiveTick(TickPhase phase) {
    const size_t tickIndex = static_cast<size_t>(phase);
    for (auto it = mTickMatrix[tickIndex].begin(); it != mTickMatrix[tickIndex].end(); ++it) {
        mComponents[it.index()]->receiveTick(phase);
    }    
}

void ComponentGroup::receiveEvent(const EventHeader* event) {
    const size_t eventIndex = static_cast<size_t>(event->type);
    for (auto it = mEventMatrix[eventIndex].begin(); it != mEventMatrix[eventIndex].end(); ++it) {
        mComponents[it.index()]->receiveEvent(event);
    }
}

} // namespace Garbox
