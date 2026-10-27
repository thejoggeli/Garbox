#include "DirtyDispatcher.h"

#include "core/assert/Assert.h"

namespace Garbox {

DirtyDispatcher::DirtyDispatcher(std::size_t capacity):
    // initialize members
    mHandlers(capacity),
    mQueue(capacity){
    // constructor body
}

void DirtyDispatcher::registerHandler(HandlerFunction function, void* context){
    const bool valid = !mHandlers.full() && function != nullptr;
    AssertExit(valid, "DirtyDispatcher", "invalid handler or capacity exhausted");
    if(!valid){
        return;
    }
    HandlerEntry entry;
    entry.function = function;
    entry.context = context;
    entry.dirty = false;
    mHandlers.push(entry);
    markDirty(mHandlers.size()-1);
}

void DirtyDispatcher::markDirty(uint32_t index){
    const bool validIndex = index < mHandlers.size();
    AssertExit(validIndex, "DirtyDispatcher", "handler index out of range");
    if(!validIndex){
        return;
    }
    HandlerEntry* entry = &mHandlers[index];
    if(!entry->dirty){
        entry->dirty = true;
        mQueue.pushBack(entry);
    }
}

void DirtyDispatcher::markAllDirty(){
    for(uint32_t idx = 0; idx < mHandlers.size(); idx++){
        markDirty(idx);
    }
}

void DirtyDispatcher::clearAllDirty(){
    HandlerEntry* entry;
    while(mQueue.releaseFront(entry)){
        entry->dirty = false;
    }
}

void DirtyDispatcher::dispatch(){
    // Callback-generated work belongs to the next pass. Bounding this pass to
    // its initial queue length prevents an unbounded self-requeue loop.
    const std::size_t dispatchCount = mQueue.size();
    HandlerEntry* entry;
    for(std::size_t index = 0; index < dispatchCount && mQueue.releaseFront(entry); ++index){
        entry->dirty = false;
        entry->function(entry->context);
    }
}

bool DirtyDispatcher::isMarkedDirty(uint32_t index) const {
    const bool validIndex = index < mHandlers.size();
    AssertExit(validIndex, "DirtyDispatcher", "handler index out of range");
    return validIndex ? mHandlers[index].dirty : false;
}

} // namespace Garbox
