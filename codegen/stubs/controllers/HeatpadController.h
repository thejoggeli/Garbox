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