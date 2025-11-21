#pragma once

#include "app/controllers/generated/HeartbeatControllerAbs.h"

namespace Garbox {

class HeartbeatController : public HeartbeatControllerAbs {
public:

    HeartbeatController(const RuntimeContext& context);

    void onHeartbeatTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox