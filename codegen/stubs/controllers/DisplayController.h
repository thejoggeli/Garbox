// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController();

    void onRenderTick() final;

    void onTemperatureStatus(const TemperatureStatus& event) final;
    void onTemperatureSample(const TemperatureSample& event) final;
    void onBacklightCommand(const BacklightCommand& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChanged& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox