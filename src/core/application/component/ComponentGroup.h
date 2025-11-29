#pragma once

#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventHeader.h"
#include "core/util/container/heap/ArrayHeap.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/heap/MaskedArrayHeap.h"
#include "shared/types/TickPhase.h"

namespace Garbox {

/**
 * Holds a fixed set of components and controls their participation in tick 
 * and event processing. 
 * 
 * Components are stored permanently and links determine which components 
 * receive specific tick phases or event types. 
 * 
 * Enabling or disabling is done per component and per tick/event category.
 */
class ComponentGroup {
public:

    ComponentGroup(std::initializer_list<ComponentAbs*> components);

    void setComponentEnabled(ComponentId id, bool enabled);
    void enableAllComponents();
    void disableAllComponents();

    void setReceiveTick(ComponentId id, TickPhase phase, bool receive);
    void setReceiveEvent(ComponentId id, EventType type, bool receive);

    void receiveTick(TickPhase phase);
    void receiveEvent(const EventHeader* event);

private:

    struct MaskEntry {
        ComponentAbs* component = nullptr;
        bool receive = false;
    };

    ArrayHeap<ComponentAbs*> mComponents; 

    /**
     * Tick and event receive matrices for components
     * ----------------------------------
     *          Comp0    Comp1    Comp2
     * Event0 [receive, receive, receive] => MaskedArray
     * Event1 [receive, receive, receive] => MaskedArray
     * Event2 [receive, receive, receive] => MaskedArray
     * Event3 [receive, receive, receive] => MaskedArray
     * ----------------------------------
     * By setting the 'receive' bool flag, it can be configure whether the
     * component at index (E, C) should receive the specific tick/event.
     * Using MaskedArray allows fast iteration over only the enabled items. 
     */
    static constexpr size_t TickPhasesCount = static_cast<size_t>(TickPhase::Count);  
    static constexpr size_t EventTypesCount = static_cast<size_t>(EventType::Count);
    ArrayStatic<MaskedArrayHeap<bool>, TickPhasesCount> mTickMatrix;
    ArrayStatic<MaskedArrayHeap<bool>, EventTypesCount> mEventMatrix;

    void setTicksEnabled(ComponentId id, bool enabled);
    void setEventsEnabled(ComponentId id, bool enabled);

};

} // namespace
