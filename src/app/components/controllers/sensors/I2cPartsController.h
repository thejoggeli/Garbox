#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/controllers/i2cparts/I2cPartsControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "core/util/fsm/FiniteStateMachine.h"

namespace Garbox {

class Gpio;
class Sht31;

class I2cPartsController : public I2cPartsControllerAbs {
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

    void handleRunningState();
    void handleStateChanged(FsmState oldState, FsmState newState);

    void updateTemperatureStatus();
    void updateTemperatureSample();

public:

// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    I2cPartsController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onSensorReadTick() final;

    // generated event handlers
    void onButtonEvent(const ButtonEvent& event) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace