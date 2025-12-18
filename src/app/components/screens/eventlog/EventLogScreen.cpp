#include "EventLogScreen.h"

#define GarboxEventLogScreenToConsole 0

#include <array>
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
    mRows(mRoot){}

void EventLogScreen::onInit(){
    for(uint32_t i = 0; i < NumRows; i++){
        LogRow& row = mRows[i];

        // setup container
        row.container.setWidth(LV_PCT(100));
        row.container.setFlexGrow(1);
        row.container.setLayout(LV_LAYOUT_FLEX);
        row.container.setFlexFlow(LV_FLEX_FLOW_ROW);
        row.container.setTextColor(DefaultTextColor);
        row.container.setFont(&lv_font_montserrat_10);

        // setup marker
        row.marker.setWidth(15);
        row.marker.setText("");
        row.marker.setTextAlign(LV_TEXT_ALIGN_CENTER);
        row.marker.setFont(&lv_font_montserrat_14);

        // setup id
        row.id.setWidth(50);
        row.id.setText("");
        row.id.setTextAlign(LV_TEXT_ALIGN_CENTER);

        // setup text
        row.text.setText("...");
    }

    setBackgroundColor(0x120034);
    mRoot.setLayout(LV_LAYOUT_FLEX);
    mRoot.setFlexFlow(LV_FLEX_FLOW_COLUMN);
    mRoot.setFlexAlign(LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
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

void EventLogScreen::onUpdateScreen(){

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