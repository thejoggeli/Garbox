#pragma once
// This stub always gets generated along, together with the remaining
// code. It can be used as a starting or reference point. 
#include "app/screens/abstract/MainScreenAbs.h"

namespace Garbox {

class MainScreen : public MainScreenAbs {
public:

    MainScreen();

    // update all user interface elements in this method
    void onUpdateScreen() final;

    // event handlers
    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;
    void onHeatpadStatus(const HeatpadStatusEvent& event) final;
    void onHeatpadSample(const HeatpadSampleEvent& event) final;
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;
    void onFermentationStatus(const FermentationStatusEvent& event) final;
    void onDisplayStatus(const DisplayStatusEvent& event) final;

private:

    void onInit() final;
    void onStart() final;
    void onBecomeEnabled() final;
    void onBecomeDisabled() final;

};

} // namespace Garbox