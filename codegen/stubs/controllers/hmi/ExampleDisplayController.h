#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/controllers/abstract/DisplayControllerAbs.h"

namespace Garbox {

class DisplayController : public DisplayControllerAbs {
public:

    DisplayController();

    void onRenderTick() final;

    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;
    void onHeatpadStatus(const HeatpadStatusEvent& event) final;
    void onHeatpadSample(const HeatpadSampleEvent& event) final;
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onDisplayCommand(const DisplayCommandEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;
    void onFermentationStatus(const FermentationStatusEvent& event) final;

private:

    void onInit() final;
    void onStart() final;

};

} // namespace Garbox