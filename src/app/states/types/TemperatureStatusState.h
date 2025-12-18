// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#pragma once

#include "core/application/state/StateAbs.h"

namespace Garbox {

class TemperatureStatusState final : public StateAbs {
public:
    TemperatureStatusState();

    // read access (published state)
    bool isDriverEnabled() const;
    bool isPowerEnabled() const;
    bool isResetting() const;
    bool isHasFirstSample() const;

    // write access (next state)
    void setDriverEnabled(bool value);
    void setPowerEnabled(bool value);
    void setResetting(bool value);
    void setHasFirstSample(bool value);

private:

    struct Data {
        bool driverEnabled;
        bool powerEnabled;
        bool resetting;
        bool hasFirstSample;
    };

    Data mCurrent {};
    Data mNext {};

    void publish() final;
};

} // namespace Garbox