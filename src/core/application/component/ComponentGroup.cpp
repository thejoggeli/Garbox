#include "ComponentGroup.h"

namespace Garbox {

ComponentGroup::ComponentGroup(std::initializer_list<ComponentAbs*> components): 
    // init members    
    mComponents(components.size()),
    mTickMatrix(components.size(), false),  // initialize ArrayMaskHeap(elementCount, defaultEnabled)
    mEventMatrix(components.size(), false){ // initialize ArrayMaskHeap(elementCount, defaultEnabled)

    // store components pointers
    std::size_t idx = 0;
    for(ComponentAbs* c : components) {
        mComponents[idx] = c;
        idx++;
    }
}

void ComponentGroup::setTicksEnabled(ComponentId id, bool enabled){
    // enable all ticks this component should receive
    const size_t componentIndex = static_cast<size_t>(id);
    for (size_t tickIndex = 0; tickIndex < static_cast<size_t>(TickPhase::Count); tickIndex++){
        if(mTickMatrix[tickIndex][componentIndex]){
            mTickMatrix[tickIndex].setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setEventsEnabled(ComponentId id, bool enabled){
    // enable all events this component should receive
    const size_t componentIndex = static_cast<size_t>(id); 
    for (size_t eventIndex = 0; eventIndex < static_cast<size_t>(EventType::Count); eventIndex++){
        if(mEventMatrix[eventIndex][componentIndex]){
            mEventMatrix[eventIndex].setIndexEnabled(componentIndex, enabled);
        }
    }
}

void ComponentGroup::setComponentEnabled(ComponentId id, bool enabled) {
    const size_t index = static_cast<size_t>(id); 
    if(mComponents[index]->isEnabled() == enabled){
        return;
    }
    mComponents[index]->setEnabled(enabled);
    setTicksEnabled(id, enabled);
    setEventsEnabled(id, enabled);
}

void ComponentGroup::enableAllComponents() {
    for(std::size_t i = 0; i < mComponents.size(); i++) {
        setComponentEnabled(static_cast<ComponentId>(i), true);
    }
}

void ComponentGroup::disableAllComponents() {
    for(std::size_t i = 0; i < mComponents.size(); i++) {
        setComponentEnabled(static_cast<ComponentId>(i), false);
    }
}

void ComponentGroup::setReceiveTick(ComponentId id, TickPhase phase, bool receive){
    const size_t componentIndex = static_cast<size_t>(id);
    const size_t tickIndex = static_cast<size_t>(phase); 
    mTickMatrix[tickIndex][componentIndex] = receive;
    if(mComponents[componentIndex]->isEnabled()){
        mTickMatrix[tickIndex].setIndexEnabled(componentIndex, receive);
    }
}

void ComponentGroup::setReceiveEvent(ComponentId id, EventType type, bool receive){
    const size_t componentIndex = static_cast<size_t>(id);
    const size_t eventIndex = static_cast<size_t>(type);
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
