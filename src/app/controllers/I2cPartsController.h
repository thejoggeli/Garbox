#pragma once

#include "app/controllers/generated/I2cPartsControllerAbs.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class Gpio;
class Sht31;

class I2cPartsController : public I2cPartsControllerAbs {
public:
    
    I2cPartsController(const RuntimeContext& context);

    void onInputTick() final;

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
    void sendTemperatureSampleEvent();

};

} // namespace