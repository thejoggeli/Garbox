#include "RowMenuRow.h"

#include <cstring>
#include "RowMenu.h"

namespace Garbox {

RowMenuRow::RowMenuRow(
    RowMenu& menu,
    LvObject& container,
    uint8_t rowIndex,
    const char* name,
    int32_t initialValue):
    // initialize members
    mMenu(menu),
    mRowIndex(rowIndex),
    mContainer(container),
    mNameLabel(mContainer),
    mValueLabel(mContainer),
    mValue(initialValue){

    // layout
    mContainer.setLayout(LV_LAYOUT_FLEX);
    mContainer.setFlexFlow(LV_FLEX_FLOW_ROW);
    mContainer.setSize(LV_PCT(100), LV_SIZE_CONTENT);
    mContainer.setTextColor(mMenu.mNormalTextColor);
    mContainer.setBgColor(mMenu.mNormalBgColor);
    mContainer.setBgOpa(LV_OPA_100);
    mContainer.setRadius(2);

    // name
    mNameLabel.setFlexGrow(1);
    mNameLabel.setTextAlign(LV_TEXT_ALIGN_LEFT);
    mNameLabel.setHeightContent();
    mNameLabel.setText(name);

    // value
    mValueLabel.setTextAlign(LV_TEXT_ALIGN_RIGHT);
    mValueLabel.setSizeContent();
    mValueLabel.setBgColor(mMenu.mNormalBgColor);
    mValueLabel.setBgOpa(LV_OPA_100);
    mValueLabel.setRadius(2);

    mNeedsRender = true;
}

void RowMenuRow::setVisible(bool visible){
    mContainer.setHidden(!visible);
}

void RowMenuRow::setActive(bool rowActive, bool valueActive){
    if(valueActive){
        mNameLabel.setTextColor(mMenu.mNormalTextColor);
        mContainer.setBgColor(mMenu.mNormalBgColor);
        mValueLabel.setTextColor(mMenu.mActiveTextColor);
        mValueLabel.setBgColor(mMenu.mActiveBgColor);
    }
    else if(rowActive){
        mNameLabel.setTextColor(mMenu.mActiveTextColor);
        mContainer.setBgColor(mMenu.mActiveBgColor);
        mValueLabel.setTextColor(mMenu.mActiveTextColor);
        mValueLabel.setBgColor(mMenu.mActiveBgColor);
    }
    else {
        mNameLabel.setTextColor(mMenu.mNormalTextColor);
        mContainer.setBgColor(mMenu.mNormalBgColor);
        mValueLabel.setTextColor(mMenu.mNormalTextColor);
        mValueLabel.setBgColor(mMenu.mNormalBgColor);
    }
}

void RowMenuRow::changeValue(int32_t delta){
    int32_t newValue = mMenu.mChangeValueCallback(mMenu.mCallbackCtx, mRowIndex, mValue, delta);
    if(mValue != newValue){
        mValue = newValue;
        mNeedsRender = true;
    }
}

void RowMenuRow::setValue(int32_t value){
    if(mValue != value){
        mValue = value;
        mNeedsRender = true;
    }
}

int32_t RowMenuRow::getValue() const{
    return mValue;
}

void RowMenuRow::render(){

    if(!mNeedsRender){
        return;
    }
    mNeedsRender = false;

    char buffer[ValueTextMaxLen] = {};
    mMenu.mValueToTextCallback(mMenu.mCallbackCtx, mRowIndex, mValue, buffer, ValueTextMaxLen);
    mValueLabel.setText(buffer);
}

void RowMenuRow::setPad(int32_t left, int32_t right, int32_t top, int32_t bottom){
    mNameLabel.setPad(left, 0, top, bottom);
    mValueLabel.setPad(left, right, top, bottom);
}

} // namespace Garbox
