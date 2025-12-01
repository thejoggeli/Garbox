// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DebugScreenAbs.h"
#include <math.h>
#include "shared/types/ComponentId.h"

namespace Garbox {

DebugScreenAbs::DebugScreenAbs(): 
    ScreenAbs(
        ComponentId::DebugScreen, 
        ScreenId::Debug, 
        static_cast<uint32_t>(Model::Index::Count)
    ),
    mModel(*this){
    // nothing to do
}

void DebugScreenAbs::init(ComponentHostIfc& host){
    ScreenAbs::init(host);
    // no model fields configured
}

DebugScreenAbs::Model& DebugScreenAbs::getModel(){
    return mModel;
}

const DebugScreenAbs::Model& DebugScreenAbs::getModel() const {
    return mModel;
}

void DebugScreenAbs::markDirty(Model::Index index){
    markDirtyRaw(static_cast<size_t>(index));
}

bool DebugScreenAbs::isMarkedDirty(Model::Index index) const {
    return isMarkedDirtyRaw(static_cast<size_t>(index));
}

DisplayCommandEvent DebugScreenAbs::makeDisplayCommandEvent(){
    return ComponentAbs::makeEvent<EventType::DisplayCommand>();
}

void DebugScreenAbs::sendEvent(const DisplayCommandEvent& event){
    publishEvent(event.header());
}

DebugScreenAbs::Model::Model(DebugScreenAbs& screen) : mScreen(screen){ 
    // nothing to do 
}

} // namespace Garbox