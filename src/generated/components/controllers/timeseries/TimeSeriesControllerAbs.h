#pragma once
// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "core/application/controller/ControllerAbs.h"
#include "generated/components/controllers/timeseries/TimeSeriesControllerStates.h"

namespace Garbox {

class Runtime;

class TimeSeriesControllerAbs : public ControllerAbs {
public:

    // component constructor
    TimeSeriesControllerAbs();

    // tick handlers (to be implmeneted by user)
    virtual void onOutputTick() = 0;

    // state change handlers (to be implmeneted by user)
    virtual void onHeatpadStatusStateChanged(const HeatpadStatusState& state) = 0;
    virtual void onTemperatureStatusStateChanged(const TemperatureStatusState& state) = 0;
    virtual void onTemperatureSampleStateChanged(const TemperatureSampleState& state) = 0;

protected:

    TimeSeriesControllerStates& states();

private:

    std::optional<TimeSeriesControllerStates> mStates;

    // hide event methods
    using ControllerAbs::makeEvent;
    using ControllerAbs::sendEventToHost;

    friend class Runtime;
};

} // namespace Garbox