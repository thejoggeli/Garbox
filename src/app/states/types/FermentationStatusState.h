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

    // write access (next state)
    void setState(FermentationState value);
    void setTargetTemperature(float value);

private:

    struct Data {
        FermentationState state;
        float targetTemperature;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox