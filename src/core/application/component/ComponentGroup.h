#pragma once

#include "core/application/component/ComponentAbs.h"
#include "core/application/event/EventHeader.h"
#include "core/util/container/heap/ArrayHeap.h"
#include "core/util/container/heap/MaskedArrayHeap.h"

namespace Garbox {

class ComponentGroup {
public:

    ComponentGroup(std::initializer_list<ComponentAbs*> components);

    void callTick(TickPhase phase);
    void callEvent(const EventHeader* event);

private:

    ArrayHeap<ComponentAbs*> mComponents; 
    MaskedArrayHeap<ComponentAbs*> mTickMask;
    MaskedArrayHeap<ComponentAbs*> mEventMask;

};

} // namespace
