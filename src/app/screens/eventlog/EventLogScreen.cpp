#include "EventLogScreen.h"

#define GarboxEventLogScreenToConsole 0

#include <array>
#include "core/log/Log.h"
#include "core/util/math/MathUtils.h"

namespace Garbox {

static const lv_color_t DefaultTextColor = lv_color_hex(0xFFFFFF);

EventLogScreen::EventLogScreen():
    EventLogScreenAbs(),
    mLabels(mRoot),
    mMarker(mRoot){
    // nothing to do
}

void EventLogScreen::onInit(){
    const int16_t startXPx = 16;
    const int16_t startYPx = 4;
    const int16_t deltaYPx = 11;
    for(uint32_t i = 0; i < NumLabels; i++){
        LvLabel& label = mLabels[i];
        label.setText("...");
        label.setPosition(startXPx, deltaYPx*i + startYPx);
        label.setTextColor(DefaultTextColor);
        label.setFont(&lv_font_montserrat_10);
    }
    setBackgroundColor(0x001234);
    mMarker.setText(">");
    mMarker.setTextColor(DefaultTextColor);
    mMarker.setFont(&lv_font_montserrat_14);
    mMarker.setPosition(-100, -100);
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
    EventEntry* entry;
    while(mBuffer.releaseFront(entry)){
        LvLabel& label = mLabels[mNextLabelIndex];
        label.setTextFormatted("[%u] %s: %s", entry->id, ComponentIdToString(entry->sender.id), EventTypeToString(entry->type));
        mMarker.setPosition(4, label.getPositionY() + label.getHeight()/2 - mMarker.getHeight()/2);
        mNextLabelIndex = MathUtils::Wrap(mNextLabelIndex+1u, NumLabels);
    }
}

void EventLogScreen::onEvent(const EventHeader* header){
#if GarboxEventLogScreenToConsole
    LogDebug("Event", "[%u] %s: %s", header->id, ComponentIdToString(header->sender.id), EventTypeToString(header->type));
#endif
    if(mBuffer.isFull()){
        mBuffer.releaseFront();
    }
    mBuffer.pushBack({header->id, header->sender, header->type});
}

} // namespace Garbox