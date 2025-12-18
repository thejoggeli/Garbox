// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"
#include "modules/parts/heatpad/HeatpadState.h"

namespace Garbox {

class HeatpadStatusState final : public StateAbs {
public:
    HeatpadStatusState();

    // read access (published state)
    HeatpadState getState() const;
    float getCurrentDutyCycle() const;
    uint32_t getCurrentPeriodMicros() const;
    float getNextDutyCycle() const;
    uint32_t getNextPeriodMicros() const;

    // read access (next state)
    HeatpadState nextState() const;
    float nextCurrentDutyCycle() const;
    uint32_t nextCurrentPeriodMicros() const;
    float nextNextDutyCycle() const;
    uint32_t nextNextPeriodMicros() const;

    // write access (next state)
    void setState(HeatpadState value);
    void setCurrentDutyCycle(float value);
    void setCurrentPeriodMicros(uint32_t value);
    void setNextDutyCycle(float value);
    void setNextPeriodMicros(uint32_t value);

private:

    struct Data {
        HeatpadState state;
        float currentDutyCycle;
        uint32_t currentPeriodMicros;
        float nextDutyCycle;
        uint32_t nextPeriodMicros;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox