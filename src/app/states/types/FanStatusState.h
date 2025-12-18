// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"
#include "modules/parts/fan/FanState.h"

namespace Garbox {

class FanStatusState final : public StateAbs {
public:
    FanStatusState();

    // read access (published state)
    FanState getState() const;
    float getTargetSpeed() const;
    bool isRpmControl() const;

    // write access (next state)
    void setState(FanState value);
    void setTargetSpeed(float value);
    void setRpmControl(bool value);

private:

    struct Data {
        FanState state;
        float targetSpeed;
        bool rpmControl;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox