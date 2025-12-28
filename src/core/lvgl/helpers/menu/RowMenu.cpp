#include "RowMenu.h"

#include "core/assert/Assert.h"

namespace Garbox {

RowMenu::RowMenu(
    LvObject& container,
    uint32_t normalTextColor,
    uint32_t activeTextColor,
    uint32_t normalBgColor,
    uint32_t activeBgColor,
    uint8_t totalRowCount,
    uint8_t visibleRowCount,
    void* callbackCtx,
    ValueToTextCallback valueToTextCallback,
    ChangeValueCallback changeValueCallback):
    // initialize members
    mContainer(container),
    mRows(totalRowCount),
    mNormalTextColor(lv_color_hex(normalTextColor)),
    mActiveTextColor(lv_color_hex(activeTextColor)),
    mNormalBgColor(lv_color_hex(normalBgColor)),
    mActiveBgColor(lv_color_hex(activeBgColor)),
    mVisibleRowCount(visibleRowCount),
    mCallbackCtx(callbackCtx),
    mValueToTextCallback(valueToTextCallback),
    mChangeValueCallback(changeValueCallback){

    mContainer.setLayout(LV_LAYOUT_FLEX);
    mContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);
    mContainer.setFlexAlign(
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );
}

void RowMenu::addRow(const char* name, int32_t initialValue){
    AssertExit((mRows.size() < mRows.capacity()), "RowMenu", "row capacity exceeded");

    const uint8_t rowIndex = static_cast<uint8_t>(mRows.size());

    mRows.emplace(
        *this,
        mContainer,
        rowIndex,
        name,
        initialValue
    );

    mNeedsRenderWindow = true;
    mNeedsRenderActive = true;
}

void RowMenu::setSelectedRow(uint8_t index){
    if(mHasSelection && index == mActiveRowIndex){
        return;
    }
    if(mLocked){
        toggleLock();
    }
    if(mActiveRowIndex >= mRows.size()){
        mActiveRowIndex = mRows.size() - 1;
    }
    mNeedsRenderWindow = true;
    mNeedsRenderActive = true;
}

void RowMenu::selectNextRow(){
    if(mLocked){
        return;
    }

    if(!mHasSelection){
        mHasSelection = true;
    }
    else if(mActiveRowIndex + 1 < mRows.size()){
        mActiveRowIndex++;
    }
    else {
        mActiveRowIndex = 0;
    }

    mNeedsRenderWindow = true;
    mNeedsRenderActive = true;
}

void RowMenu::selectPreviousRow(){
    if(mLocked){
        return;
    }

    if(!mHasSelection){
        mHasSelection = true;
    }
    else if(mActiveRowIndex > 0){
        mActiveRowIndex--;
    }
    else {
        mActiveRowIndex = mRows.size() - 1;
    }

    mNeedsRenderWindow = true;
    mNeedsRenderActive = true;
}

void RowMenu::toggleLock(){
    if(!mHasSelection){
        return;
    }
    mLocked = !mLocked;
    mNeedsRenderActive = true;
}

void RowMenu::nextValue(){
    if(!mLocked){
        return;
    }
    mRows[mActiveRowIndex].changeValue(+1);
}

void RowMenu::previousValue(){
    if(!mLocked){
        return;
    }
    mRows[mActiveRowIndex].changeValue(-1);
}

void RowMenu::clearSelection(){
    mHasSelection = false;
    mLocked = false;
    mNeedsRenderActive = true;
}

int32_t RowMenu::getValue(uint8_t rowIndex) const{
    AssertExit((rowIndex < mRows.size()), "RowMenu", "row index out of range");
    return mRows[rowIndex].getValue();
}

void RowMenu::setValue(uint8_t rowIndex, int32_t value){
    AssertExit((rowIndex < mRows.size()), "RowMenu", "row index out of range");
    mRows[rowIndex].setValue(value);
}

void RowMenu::render(){
    renderWindow();
    renderActive();
    for(int i = 0; i < mRows.size(); i++){
        mRows[i].render();
    }
}

void RowMenu::renderWindow(){

    if(!mNeedsRenderWindow){
        return;
    }
    mNeedsRenderWindow = false;

    if(mRows.size() <= mVisibleRowCount){
        mWindowStartIndex = 0;
        return;
    }

    const int32_t half = static_cast<int32_t>(mVisibleRowCount / 2);

    int32_t start = static_cast<int32_t>(mActiveRowIndex) - half;

    if(start < 0){
        start = 0;
    }

    const int32_t maxStart = static_cast<int32_t>(mRows.size() - mVisibleRowCount);

    if(start > maxStart){
        start = maxStart;
    }

    mWindowStartIndex = static_cast<uint8_t>(start);
}

void RowMenu::renderActive(){

    if(!mNeedsRenderActive){
        return; 
    }
    mNeedsRenderActive = false;

    for(uint8_t i = 0; i < mRows.size(); i++){

        const bool visible = (i >= mWindowStartIndex) && (i < mWindowStartIndex + mVisibleRowCount);
        mRows[i].setVisible(visible);

        if(!mHasSelection){
            mRows[i].setActive(false, false);
            continue;
        }

        const bool isActive = (i == mActiveRowIndex);
        const bool valueActive = isActive && mLocked;
        mRows[i].setActive(isActive, valueActive);
    }
}

void RowMenu::setPad(int32_t left, int32_t right, int32_t top, int32_t bottom){
    for(uint8_t i = 0; i < mRows.size(); i++){
        mRows[i].setPad(left, right, top, bottom);
    }
}

} // namespace Garbox
