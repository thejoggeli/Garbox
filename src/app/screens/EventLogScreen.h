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

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;
    void onUpdateScreen() final; // safe to update ui elements in this method

};

} // namespace Garbox