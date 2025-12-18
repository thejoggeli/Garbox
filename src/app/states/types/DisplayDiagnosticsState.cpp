// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "DisplayDiagnosticsState.h"

namespace Garbox {

DisplayDiagnosticsState::DisplayDiagnosticsState(): StateAbs(StateType::DisplayDiagnostics){
    // mCurrent and mNext default-initialized
}

uint32_t DisplayDiagnosticsState::getSkippedFrames() const {
    return mCurrent.skippedFrames;
}

uint32_t DisplayDiagnosticsState::nextSkippedFrames() const {
    return mCurrent.skippedFrames;
}

void DisplayDiagnosticsState::setSkippedFrames(uint32_t value){
    if (mNext.skippedFrames == value) {
        return;
    }
    mNext.skippedFrames = value;
    markDirty();
}

void DisplayDiagnosticsState::publish(){
    if (!isDirty()) {
        return;
    }
    mCurrent = mNext;
    clearDirty();
}

} // namespace Garbox