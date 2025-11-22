// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/behaviours/generated/CalibrationBehaviourAbs.h"

namespace Garbox {

class CalibrationBehaviour : public CalibrationBehaviourAbs {
public:

    CalibrationBehaviour();

    void onLogicTick() final;

    void onHeartbeat(const Heartbeat& event) final;
    void onFanStatus(const FanStatus& event) final;
    void onFanSample(const FanSample& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeActive() final;
    void onBecomeInactive() final;

};

} // namespace Garbox