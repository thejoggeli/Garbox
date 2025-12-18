#pragma once

#include "app/generated/screens/EventLogScreenAbs.h"
#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/static/RingBufferStatic.h"

namespace Garbox {

class EventLogScreen : public EventLogScreenAbs {
public:

    EventLogScreen();

    // event handler
    void onEvent(const EventHeader* header) final;

private:

    struct EventEntry {
        uint32_t id;
        ComponentDescriptor sender;
        EventType type;
    };

    struct LogRow {
        LvObject container;
        LvLabel marker;
        LvLabel id;
        LvLabel text;
        LogRow(LvObject& parent);
    };

    static constexpr size_t NumRows = 22;
    ArrayStatic<LogRow, NumRows> mRows;
    size_t mNextLabelIndex = 0;
    size_t mCurrentLabelIndex = static_cast<size_t>(-1);
    RingBufferStatic<EventEntry, NumRows> mBuffer;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

};

} // namespace Garbox