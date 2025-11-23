#pragma once

#include "app/controllers/generated/DisplayControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "core/util/helpers/TimeFader.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class Display;

class DisplayController : public DisplayControllerAbs {
public:

    struct ShadowState {
        FanState fanState;
        float fanTargetSpeed;
        float fanMeasuredRpm;
        HeatpadState heatpadState;
        float heatpadDuty;
        uint32_t heatpadPeriod;
        float heatpadVoltage;
        float heatpadCurrent;
        bool shtPower;
        bool shtDriver;
        bool shtReset;
        float shtTemp;
        float shtHum;
        uint32_t renderSkippedCount = 0;
        float brightness;
        uint32_t heapSpace = 0;
        BehaviourId behaviour;
        uint32_t eventCount = 0;
    };

    struct Dirty {
        bool displayState = true;
        bool fanStatus = true;
        bool fanMeasuredRpm = true;
        bool heatpadState = true;
        bool heatpadDuty = true;
        bool heatpadSense = true;
        bool shtState = true;
        bool shtSample = true;
        bool appState = true;
        bool heapSpace = true;
    };
    
    DisplayController();

    void onRenderTick() final;

    void onFanStatus(const FanStatusEvent& event) final;
    void onFanSample(const FanSampleEvent& event) final;
    void onHeatpadStatus(const HeatpadStatusEvent& event);
    void onHeatpadSample(const HeatpadSampleEvent& event);
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onBacklightCommand(const BacklightCommandEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;

private:

    Display& mDisplay;
    SoftwareTimer mHeapTimer;
    TimeFader mBacklightFader;

    Dirty mDirtyFlags;
    ShadowState mShadowState {};    

    uint32_t mRenderSkippedCount = 0;

    void onInit() final;
    void onStart() final;

    void setBrightnessSmooth(float brightness, uint32_t durationMicros);

};

} // namespace