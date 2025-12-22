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
    bool getDriverEnabled() const;
    bool getPowerEnabled() const;
    bool getResetting() const;
    bool getHasFirstSample() const;
    bool getIsRunning() const;

    // read access (next state)
    bool nextDriverEnabled() const;
    bool nextPowerEnabled() const;
    bool nextResetting() const;
    bool nextHasFirstSample() const;
    bool nextIsRunning() const;

    // read access (last state)
    bool lastDriverEnabled() const;
    bool lastPowerEnabled() const;
    bool lastResetting() const;
    bool lastHasFirstSample() const;
    bool lastIsRunning() const;

    // write access (next state)
    void setDriverEnabled(bool value);
    void setPowerEnabled(bool value);
    void setResetting(bool value);
    void setHasFirstSample(bool value);
    void setIsRunning(bool value);

private:

    struct Data {
        bool driverEnabled;
        bool powerEnabled;
        bool resetting;
        bool hasFirstSample;
        bool isRunning;
    };

    Data mCurrent {};
    Data mNext {};
    Data mLast {};

    void publish() final;
};

} // namespace Garbox