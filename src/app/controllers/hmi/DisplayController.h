#pragma once

#include <functional>
#include "app/controllers/abstract/DisplayControllerAbs.h"
#include "core/time/SoftwareTimer.h"
#include "core/util/container/vector/StaticVector.h"
#include "core/util/helpers/TimeFader.h"
#include "modules/parts/fan/FanState.h"
#include "modules/parts/heatpad/HeatpadState.h"
#include "shared/types/BehaviourId.h"

namespace Garbox {

class Display;
class LvglObjects;

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

    enum class Index : uint8_t {
        FanStatus = 0,
        FanMeasuredRpm,
        HeatpadState,
        HeatpadDuty,
        HeatpadSense,
        DisplayState,
        ShtState,
        ShtSample,
        HeapSpace,
        AppState,
        Count,
    };

    using UpdateFunction = std::function<void()>;

    struct UpdateHandler {
        bool dirty = false;
        UpdateFunction updateFn;

        // constructor for emplace
        UpdateHandler(bool d, UpdateFunction fn): dirty(d), updateFn(fn) {}
    };

    Display& mDisplay;
    LvglObjects& mObjects;

    SoftwareTimer mHeapTimer;
    TimeFader mBacklightFader;

    ShadowState mShadowState {};

    uint32_t mRenderSkippedCount = 0;
    uint32_t mDirtyCount = 0;

    static constexpr size_t MaxUpdateHandlers = static_cast<size_t>(Index::Count);
    StaticVector<UpdateHandler, MaxUpdateHandlers> mUpdateHandlers;
    StaticVector<UpdateHandler*, MaxUpdateHandlers> mDirtyUpdateHandlers;

    void onInit() final;
    void onStart() final;

    void setBrightnessSmooth(float brightness, uint32_t durationMicros);

    void registerHandler(UpdateFunction function);
    void registerHandlers();
    void markDirty(Index index);

};

} // namespace