#pragma once

#include "app/screens/abstract/EventLogScreenAbs.h"
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

    static constexpr size_t NumLabels = 21;
    ArrayStatic<LvLabel, NumLabels> mLabels;
    size_t mNextLabelIndex = 0;
    RingBufferStatic<EventEntry, NumLabels> mBuffer;
    LvLabel mMarker;

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final;

};

} // namespace Garbox