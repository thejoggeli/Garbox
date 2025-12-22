#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/screens/EventLogScreenAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/lvgl/objects/LvObject.h"
#include "core/lvgl/objects/LvLabel.h"
#include "core/util/container/static/ArrayStatic.h"
#include "core/util/container/static/RingBufferStatic.h"

namespace Garbox {

class EventLogScreen : public EventLogScreenAbs {
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

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    EventLogScreen();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

    // generated event handlers
    void onEvent(const EventHeader* header) final;

    // generated render functions
    void onRender() final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox