#include "DirtyDispatcher.h"

#include "core/assert/Assert.h"

namespace Garbox {

DirtyDispatcher::DirtyDispatcher(size_t capacity):
    // initialize members
    mHandlers(capacity),
    mQueue(capacity){
    // constructor body
}

void DirtyDispatcher::registerHandler(HandlerFunction function, void* context){
    AssertExit(!mHandlers.full(), "DirtyDispatcher", "out of handler capacity");
    HandlerEntry entry;
    entry.function = function;
    entry.context = context;
    entry.dirty = false;
    mHandlers.push(entry);
    markDirty(mHandlers.size()-1);
}

void DirtyDispatcher::markDirty(uint32_t index){
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
    HandlerEntry* entry;
    while(mQueue.releaseFront(entry)){
        entry->function(entry->context);
        entry->dirty = false;
    }
}

} // namespace Garbox
