#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/HeartbeatControllerAbs.h"

namespace Garbox {

class HeartbeatController : public HeartbeatControllerAbs {
public:

    HeartbeatController();

    void onHeartbeatTick() final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox