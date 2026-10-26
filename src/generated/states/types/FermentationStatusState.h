// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"
#include "app/engine/FermentationState.h"

namespace Garbox {

class FermentationStatusState final : public StateAbs {
public:
    FermentationStatusState();

    // read access (published state)
    FermentationState getState() const;
    float getTargetTemperature() const;
    bool getFanAuto() const;
    float getFanManualSpeed() const;
    bool getPowerOn() const;

    // read access (next state)
    FermentationState nextState() const;
    float nextTargetTemperature() const;
    bool nextFanAuto() const;
    float nextFanManualSpeed() const;
    bool nextPowerOn() const;

    // read access (last state)
    FermentationState lastState() const;
    float lastTargetTemperature() const;
    bool lastFanAuto() const;
    float lastFanManualSpeed() const;
    bool lastPowerOn() const;

    // write access (next state)
    void setState(FermentationState value);
    void setTargetTemperature(float value);
    void setFanAuto(bool value);
    void setFanManualSpeed(float value);
    void setPowerOn(bool value);

private:

    struct Data {
        FermentationState state;
        float targetTemperature;
        bool fanAuto = true;
        float fanManualSpeed = 0.0f;
        bool powerOn = false;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox