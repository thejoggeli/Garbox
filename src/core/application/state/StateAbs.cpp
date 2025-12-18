#include "StateAbs.h"

#include "core/assert/Assert.h"

namespace Garbox {

StateAbs::StateAbs(StateType type) : mType(type){
    // nothing to do
}

bool StateAbs::isDirty() const {
    return mDirty;
}

void StateAbs::markDirty(){
    // only mark dirty on transitions to true
    if(mDirty){
        return;
    }
    mDirty = true;

    // notify host that this state became dirty
    if (mHost) {
        mHost->markStateDirty(this);
    }
}

void StateAbs::clearDirty(){
    mDirty = false;
}

StateType StateAbs::type() const {
    return mType;
}

void StateAbs::init(StateHostIfc* host){
    AssertExit(mHost == nullptr, "StateAbs", "already initialized");
    AssertExit(host != nullptr, "StateAbs", "passed nullptr");
    mHost = host;
}

} // namespace Garbox
