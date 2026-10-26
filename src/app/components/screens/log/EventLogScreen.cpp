#include "EventLogScreen.h"

#define GarboxEventLogScreenToConsole 0

#include "app/components/screens/ScreenCommandBar.h"
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

static const lv_color_t DefaultTextColor = lv_color_hex(0xFFFFFF);

EventLogScreen::LogRow::LogRow(LvObject& parent):
    container(parent),
    marker(container),
    id(container),
    text(container){}

EventLogScreen::EventLogScreen():
    EventLogScreenAbs(),
    mHeader(mRoot),
    mHeaderTitle(mHeader),
    mViewHint(mHeader),
    mRows(mRoot),
    mFooter(mRoot),
    mFooterPressHint(mFooter),
    mFooterHoldHint(mFooter){}

void EventLogScreen::onInit(){
    mHeader.setWidth(LV_PCT(100));
    mHeader.setHeight(22);
    mHeader.setPad(6, 6, 4, 2);
    mHeader.setLayout(LV_LAYOUT_FLEX);
    mHeader.setFlexFlow(LV_FLEX_FLOW_ROW);
    mHeader.setFlexAlign(LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    mHeaderTitle.setText("EVENT LOG");
    mHeaderTitle.setFont(&lv_font_montserrat_12);
    mHeaderTitle.setTextColor(lv_color_hex(0xFF9CB8));
    mViewHint.setText("ROTATE VIEWS 2/3");
    mViewHint.setFont(&lv_font_unscii_8);
    mViewHint.setTextColor(lv_color_hex(0x8AB4F4));

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
        "PRESS: STATE LOG",
        "HOLD: MAIN"
    );

    setBackgroundColor(0x160910);
    mRoot.setLayout(LV_LAYOUT_FLEX);
    mRoot.setFlexFlow(LV_FLEX_FLOW_COLUMN);
    mRoot.setFlexAlign(LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
}

void EventLogScreen::onStart(){
    // nothing to do
}

void EventLogScreen::onBecomeEnabled(){
    // nothing to do
}

void EventLogScreen::onBecomeDisabled(){
    // nothing to do
}

void EventLogScreen::onButtonEvent(const ButtonEvent& event){
    if(event->newState == ButtonState::PressedLong){
        host()->requestChangeScreen(ScreenId::Main);
    }
    else if(event->newState == ButtonState::Released && event->oldState == ButtonState::Pressed){
        host()->requestChangeScreen(ScreenId::StateLog);
    }
}

void EventLogScreen::onEncoderStepEvent(const EncoderStepEvent& event){
    if(event->steps > 0){
        host()->requestChangeScreen(ScreenId::StateLog);
    }
    else if(event->steps < 0){
        host()->requestChangeScreen(ScreenId::Debug);
    }
}

void EventLogScreen::onRender(){

    // write queued events to labels
    if(mBuffer.size() > 0){
        EventEntry* entry;
        LogRow* firstRow = (mCurrentLabelIndex == static_cast<size_t>(-1)) ? nullptr : &mRows[mCurrentLabelIndex];
        LogRow* lastRow = nullptr;
        while(mBuffer.releaseFront(entry)){
            LogRow& row = mRows[mNextLabelIndex];
            row.id.setTextFormatted("%u", entry->id);
            row.text.setTextFormatted("%s: %s", ComponentIdToString(entry->sender.id), EventTypeToString(entry->type));
            lastRow = &row;
            mCurrentLabelIndex = mNextLabelIndex;
            mNextLabelIndex = MathUtils::Wrap(mNextLabelIndex+1u, NumRows);
        }

        // update marker
        AssertExit(lastRow != nullptr, "EventLogScreen", "lastRow is nullptr");
        if(firstRow != nullptr){
            firstRow->marker.setText("");
        }
        lastRow->marker.setText(">");
        
        // grayscale gradient
        // disabled because it slows down frame rate like crazy
        // int idx = mNextLabelIndex;
        // for(int i = 0; i < NumRows; i++){
        //     idx = MathUtils::Wrap(idx+1u, NumRows);
        //     int32_t gray = MathUtils::Map<int32_t>(i, 0, (NumRows-1), 128, 255);
        //     lv_color_t c = lv_color_make(gray, gray, gray);
        //     mRows[idx].container.setTextColor(c);
        // }
    }
}

void EventLogScreen::onEvent(const EventHeader* header){
#if GarboxEventLogScreenToConsole
    LogDebug("Event", "%u %s: %s", header->id, ComponentIdToString(header->sender.id), EventTypeToString(header->type));
#endif
    if(mBuffer.isFull()){
        mBuffer.releaseFront();
    }
    mBuffer.pushBack({header->id, header->sender, header->type});
}

} // namespace Garbox
