// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController(const RuntimeContext& context);

    void onRenderTick() final;

    void onTemperatureStatus(const EventRead<EventPayload::TemperatureStatus>& event) final;
    void onTemperatureSample(const EventRead<EventPayload::TemperatureSample>& event) final;
    void onBacklightCommand(const EventRead<EventPayload::BacklightCommand>& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox