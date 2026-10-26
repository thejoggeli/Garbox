#include "StateLogScreen.h"

#include "app/components/screens/ScreenCommandBar.h"
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

static const lv_color_t DefaultTextColor = lv_color_hex(0xFFFFFF);

StateLogScreen::LogRow::LogRow(LvObject& parent):
    container(parent),
    marker(container),
    id(container),
    text(container){}

StateLogScreen::StateLogScreen(): 
    StateLogScreenAbs(), 
    mHeader(mRoot),
    mRows(mRoot),
    mFooter(mRoot),
    mFooterPressHint(mFooter),
    mFooterHoldHint(mFooter){}

void StateLogScreen::onInit(){
    mHeader.setText("STATE LOG");
    mHeader.setWidth(LV_PCT(100));
    mHeader.setHeight(22);
    mHeader.setPad(6, 6, 4, 2);
    mHeader.setFont(&lv_font_montserrat_12);
    mHeader.setTextColor(lv_color_hex(0xBDA7FF));

    for(uint32_t i = 0; i < NumRows; i++){
        LogRow& row = mRows[i];

        // setup container
        row.container.setWidth(LV_PCT(100));
        row.container.setFlexGrow(1);
        row.container.setLayout(LV_LAYOUT_FLEX);
        row.container.setFlexFlow(LV_FLEX_FLOW_ROW);
        row.container.setScrollable(false);
        lv_obj_set_scrollbar_mode(row.container.raw(), LV_SCROLLBAR_MODE_OFF);
        row.container.setTextColor(DefaultTextColor);
        row.container.setFont(&lv_font_montserrat_10);

        // setup marker
        row.marker.setWidth(15);
        row.marker.setText("");
        row.marker.setTextAlign(LV_TEXT_ALIGN_CENTER);
        row.marker.setFont(&lv_font_montserrat_14);

        // setup id
        row.id.setWidth(42);
        row.id.setText("");
        row.id.setTextAlign(LV_TEXT_ALIGN_CENTER);

        // setup text
        row.text.setText("...");
    }

    ConfigureScreenCommandBar(
        mFooter,
        mFooterPressHint,
        mFooterHoldHint,
        "PRESS: DIAGNOSTICS",
        "HOLD: MAIN"
    );

    setBackgroundColor(0x0C0918);
    mRoot.setLayout(LV_LAYOUT_FLEX);
    mRoot.setFlexFlow(LV_FLEX_FLOW_COLUMN);
    mRoot.setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
}

void StateLogScreen::onStart(){
    // nothing to do
}

void StateLogScreen::onBecomeEnabled(){
    // nothing to do
}

void StateLogScreen::onBecomeDisabled(){
    // nothing to do
}

void StateLogScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::PressedLong){
        host()->requestChangeScreen(ScreenId::Main);
    }
    else if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        host()->requestChangeScreen(ScreenId::Debug);
    }
}

void StateLogScreen::onRender(){

    // write queued events to labels
    if(mBuffer.size() > 0){
        StateEntry* entry;
        LogRow* firstRow = (mCurrentLabelIndex == static_cast<size_t>(-1)) ? nullptr : &mRows[mCurrentLabelIndex];
        LogRow* lastRow = nullptr;
        while(mBuffer.releaseFront(entry)){
            LogRow& row = mRows[mNextLabelIndex];
            row.id.setTextFormatted("%u", entry->id);
            row.text.setText(StateTypeToString(entry->type));
            lastRow = &row;
            mCurrentLabelIndex = mNextLabelIndex;
            mNextLabelIndex = MathUtils::Wrap(mNextLabelIndex+1u, NumRows);
        }

        // update marker
        AssertExit(lastRow != nullptr, "StateLogScreen", "lastRow is nullptr");
        if(firstRow != nullptr){
            firstRow->marker.setText("");
        }
        lastRow->marker.setText(">");
    }
}
void StateLogScreen::onStateChanged(const StateAbs& state){
    if(mBuffer.isFull()){
        mBuffer.releaseFront();
    }
    mBuffer.pushBack({ mEntryCounter, state.type() });
    mEntryCounter++;
}

} // namespace Garbox
