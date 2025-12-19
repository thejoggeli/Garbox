#pragma once

#include "core/util/fsm/FiniteStateMachine.h"
#include "generated/controllers/I2cPartsControllerAbs.h"

namespace Garbox {

class Gpio;
class Sht31;

class I2cPartsController : public I2cPartsControllerAbs {
public:
    
    I2cPartsController();

    void onInputTick() final;
    void onButtonEvent(const ButtonEvent& event) final;

private:

    enum class FsmState {
        ResetPowerOffPhase = 0,
        ResetPowerOnPhase,
        Running,
        Count
    };

    static constexpr uint32_t ResetPowerOffPhaseDurationMicros = 100_ms;
    static constexpr uint32_t ResetPowerOnPhaseDurationMicros = 1000_ms;
    static constexpr uint32_t MaxFetchDurationBeforeRestartMicros = 5000_ms;
    FiniteStateMachine<FsmState, FsmState::Count> mFsm;

    Gpio& mEnablePowerGpio;
    Sht31& mTemperatureSensor;

    bool mHasFirstSample = false;
    bool mNewSample = false;
    bool mStateChanged = true;
    bool mResetting = false;
    
    void onInit() final;
    void onStart() final;

    void handleRunningState();
    void handleStateChanged(FsmState oldState, FsmState newState);

    void updateTemperatureStatus();
    void updateTemperatureSample();

};

} // namespace