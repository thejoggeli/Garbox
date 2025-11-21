// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/HeatpadControllerAbs.h"

namespace Garbox {

class HeatpadController : public HeatpadControllerAbs {
public:

    HeatpadController(const RuntimeContext& context);

    void onInputTick() final;
    void onOutputTick() final;

    void onHeatpadCommand(const EventRead<EventPayload::HeatpadCommand>& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox