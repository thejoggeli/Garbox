// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ActiveBehaviourState.h"

namespace Garbox {

ActiveBehaviourState::ActiveBehaviourState(): StateAbs(StateType::ActiveBehaviour){
    // mCurrent and mNext default-initialized
}

BehaviourId ActiveBehaviourState::getBehaviour() const {
    return mCurrent.behaviour;
}

BehaviourId ActiveBehaviourState::nextBehaviour() const {
    return mCurrent.behaviour;
}

void ActiveBehaviourState::setBehaviour(BehaviourId value){
    if (mNext.behaviour == value) {
        return;
    }
    mNext.behaviour = value;
    markDirty();
}

void ActiveBehaviourState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox