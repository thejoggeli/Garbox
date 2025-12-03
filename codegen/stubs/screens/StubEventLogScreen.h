#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/EventLogScreenAbs.h"

namespace Garbox {

class EventLogScreen : public EventLogScreenAbs {
public:

    EventLogScreen();

    // event handler
    void onEvent(const EventHeader* header) final;

private:

    // safe to update lvgl objects in these methods
    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; 

};

} // namespace Garbox