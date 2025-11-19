#pragma once

#include "core/application/controller/ControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class Gpio;
class Sht31;

class I2cPartsController : public ControllerAbs {
public:
    
    I2cPartsController(ComponentId id);

    void onInputTick();
    void onOutputTick();

private:

    enum class FsmState {
        ResetPowerOffPhase = 0,
        ResetPowerOnPhase,
        Running,
    };

    Gpio& mEnablePowerGpio;
    Sht31& mTemperatureSensor;

    FsmState mFsmState = FsmState::ResetPowerOffPhase;

    bool mChanged = true;

    static constexpr uint32_t ResetPowerOffPhaseDurationMicros = 100;
    static constexpr uint32_t ResetPowerOnPhaseDurationMicros = 1000;
    SoftwareTimer mResetTimer;
    
    void onInit() final;
    void onStart() final;

    void enterFsmState(FsmState state);
    void sendTemperatureStatusEvent();

};

} // namespace