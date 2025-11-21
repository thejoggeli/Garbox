#pragma once

#include "app/controllers/generated/FanControllerAbs.h"

namespace Garbox {

class FanController : public FanControllerAbs {
public:

    FanController(const RuntimeContext& context);

    void onInputTick() final;
    void onOutputTick() final;

    void onFanCommand(const EventRead<EventPayload::FanCommand>& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox