// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 

#pragma once

#include "app/controllers/generated/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController();

    void onRenderTick() final;

    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onBacklightCommand(const BacklightCommandEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox