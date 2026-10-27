#include "RowMenu.h"

#include "core/assert/Assert.h"

namespace Garbox {

RowMenu::RowMenu(
    LvObject& container,
    uint32_t normalTextColor,
    uint32_t activeTextColor,
    uint32_t normalBgColor,
    uint32_t activeBgColor,
    uint32_t editTextColor,
    uint32_t editBgColor,
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
    mEditTextColor(lv_color_hex(editTextColor)),
    mEditBgColor(lv_color_hex(editBgColor)),
    mVisibleRowCount(visibleRowCount > 0 ? visibleRowCount : 1),
    mCallbackCtx(callbackCtx),
    mValueToTextCallback(valueToTextCallback ? valueToTextCallback : fallbackValueToText),
    mChangeValueCallback(changeValueCallback ? changeValueCallback : fallbackChangeValue){

    AssertExit(visibleRowCount > 0, "RowMenu", "visible row count must be positive");
    AssertExit(valueToTextCallback != nullptr, "RowMenu", "value-to-text callback is required");
    AssertExit(changeValueCallback != nullptr, "RowMenu", "change-value callback is required");

    mContainer.setLayout(LV_LAYOUT_FLEX);
    mContainer.setFlexFlow(LV_FLEX_FLOW_COLUMN);
    mContainer.setFlexAlign(
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER
    );
}

void RowMenu::addRow(const char* name, int32_t initialValue){
    if(mRows.full()){
        TriggerExit("RowMenu", "row capacity exceeded");
        return;
    }
    if(name == nullptr){
        TriggerExit("RowMenu", "row name is nullptr");
        return;
    }

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
    AssertExit((index < mRows.size()), "RowMenu", "row index out of range");
    if(index >= mRows.size()) return;
    if(mLocked){
        toggleLock();
    }
    mActiveRowIndex = index;
    mHasSelection = true;
    mNeedsRenderWindow = true;
    mNeedsRenderActive = true;
}

void RowMenu::selectNextRow(){
    if(mLocked){
        return;
    }
    if(mRows.empty()){
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
    if(mRows.empty()){
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

void RowMenu::changeSelectedValue(int32_t delta){
    if(!mHasSelection){
        return;
    }
    mRows[mActiveRowIndex].changeValue(delta);
}

void RowMenu::clearSelection(){
    mHasSelection = false;
    mLocked = false;
    mNeedsRenderActive = true;
}

bool RowMenu::hasSelection() const {
    return mHasSelection;
}

bool RowMenu::isLocked() const {
    return mLocked;
}

uint8_t RowMenu::getSelectedRow() const {
    AssertExit(mHasSelection, "RowMenu", "menu has no selection");
    return mHasSelection ? mActiveRowIndex : 0;
}

uint8_t RowMenu::getWindowStartRow() const {
    return mWindowStartIndex;
}

int32_t RowMenu::getValue(uint8_t rowIndex) const{
    AssertExit((rowIndex < mRows.size()), "RowMenu", "row index out of range");
    if(rowIndex >= mRows.size()) return 0;
    return mRows[rowIndex].getValue();
}

void RowMenu::setValue(uint8_t rowIndex, int32_t value){
    AssertExit((rowIndex < mRows.size()), "RowMenu", "row index out of range");
    if(rowIndex >= mRows.size()) return;
    mRows[rowIndex].setValue(value);
}

void RowMenu::render(){
    renderWindow();
    renderActive();
    for(std::size_t i = 0; i < mRows.size(); i++){
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

void RowMenu::fallbackValueToText(
    void*, uint8_t, int32_t, char* outText, uint32_t maxLen){
    if(outText != nullptr && maxLen > 0) outText[0] = '\0';
}

int32_t RowMenu::fallbackChangeValue(
    void*, uint8_t, int32_t currentValue, int32_t){
    return currentValue;
}

} // namespace Garbox
