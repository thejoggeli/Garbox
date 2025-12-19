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

ScreenId ActiveScreenState::nextScreen() const {
    return mCurrent.screen;
}

ScreenId ActiveScreenState::lastScreen() const {
    return mLast.screen;
}

void ActiveScreenState::setScreen(ScreenId value){
    if (mNext.screen == value) {
        return;
    }
    mNext.screen = value;
    markDirty();
}

void ActiveScreenState::publish(){
    if (!isDirty()) {
        return;
    }
    mLast = mCurrent;
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox