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

    struct State {
        FanState fanState;
        float fanTargetSpeed;
        float fanMeasuredRpm;
        HeatpadState heatpadState;
        float heatpadDuty;
        float heatpadVoltage;
        float heatpadCurrent;
        bool shtPower;
        bool shtDriver;
        bool shtReset;
        float shtTemp;
        float shtHum;
        uint32_t renderSkippedCount = 0xFFFFFFFF;
        float brightness;
        uint32_t heapSpace = 0;
        BehaviourId behaviour;
        uint32_t eventCount = 0xFFFFFFFF;
    };

    struct Dirty {
        bool shtState = true;
        bool shtSample = true;
    };
    
    DisplayController();

    void onRenderTick() final;
    void onTemperatureStatus(const TemperatureStatusEvent& event) final;
    void onTemperatureSample(const TemperatureSampleEvent& event) final;
    void onBacklightCommand(const BacklightCommandEvent& event) final;
    void onActiveBehaviourChanged(const ActiveBehaviourChangedEvent& event) final;

private:

    Display& mDisplay;
    SoftwareTimer mHeapTimer;
    TimeFader mBacklightFader;

    Dirty mDirty;
    State mOldState {};
    State mNewState {};    

    uint32_t mRenderSkippedCount = 0;

    void onInit() final;
    void onStart() final;

    void setBrightnessSmooth(float brightness, uint32_t durationMicros);

};

} // namespace