// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "ActiveScreenState.h"

namespace Garbox {

ActiveScreenState::ActiveScreenState(): StateAbs(StateType::ActiveScreen){
    // mCurrent and mNext default-initialized
}

ScreenId ActiveScreenState::getScreen() const {
    return mCurrent.screen;
}

void ActiveScreenState::setScreen(ScreenId value){
    if (mNext.screen == value) {
        return;
    }
    mNext.screen = value;
    setDirty();
}

void ActiveScreenState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
}

} // namespace Garbox