#pragma once

#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventHeader.h"
#include "core/util/container/heap/ArrayHeap.h"
#include "core/util/container/heap/MaskedArrayHeap.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/Span.h"
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
 * 
 * Lists of MaskedArray's of the following shapes are used to store the event routing:
 * - TickPhases x Components
 * - EventTypes x Components
 * ----------------------------------
 *          Comp0    Comp1    Comp2
 * Event0 [receive, receive, receive] => MaskedArray
 * Event1 [receive, receive, receive] => MaskedArray
 * Event2 [receive, receive, receive] => MaskedArray
 * Event3 [receive, receive, receive] => MaskedArray
 * ----------------------------------
 * Using MaskedArray allows fast iteration over only the enabled items. 
 */
class ComponentGroup {
public:

    ComponentGroup();

    void init(std::initializer_list<ComponentAbs*> components);
    Span<ComponentAbs*> getComponents();

    void setComponentEnabled(ComponentAbs* component, bool enabled);
    void enableAllComponents();
    void disableAllComponents();

    void setRouteTick(ComponentAbs* component, TickPhase phase, bool receive);
    void setRouteEvent(ComponentAbs* component, EventType type, bool receive);

    void routeTick(TickPhase phase);
    void routeEvent(const EventHeader* event);

    bool isInitialized() const { return mInitialized; }

private:

    bool mInitialized = false;

    static constexpr size_t TickPhasesCount = static_cast<size_t>(TickPhase::Count);  
    static constexpr size_t EventTypesCount = static_cast<size_t>(EventType::Count);

    using ComponentsArray = ArrayHeap<ComponentAbs*>;
    using TicksMatrix = ArrayStatic<MaskedArrayHeap<bool>, TickPhasesCount>;
    using EventsMatrix = ArrayStatic<MaskedArrayHeap<bool>, EventTypesCount>;
 
    ComponentsArray* mComponents; 
    TicksMatrix* mTicksMatrix;
    EventsMatrix* mEventsMatrix;

    void setTicksEnabled(size_t componentIndex, bool enabled);
    void setEventsEnabled(size_t componentIndex, bool enabled);

};

} // namespace
