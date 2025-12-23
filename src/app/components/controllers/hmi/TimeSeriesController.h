#pragma once

// ==== GENERATED BEGIN: include ==================================================
#include "generated/components/controllers/timeseries/TimeSeriesControllerAbs.h"
// ==== GENERATED END: include ====================================================

#include "app/services/GarboxHistory.h"
#include "core/time/SoftwareTimer.h"

namespace Garbox {

class TimeSeriesController : public TimeSeriesControllerAbs {
private:

    GarboxHistory& mHistory;
    bool mStarted = false;

public:
// ==== GENERATED BEGIN: interface ================================================

    // generated constructor
    TimeSeriesController();

    // generated lifecycle handlers
    void onInit() final;
    void onStart() final;

    // generated tick handlers
    void onOutputTick() final;

    // generated state changed handlers
    void onHeatpadStatusStateChanged(const HeatpadStatusState& state) final;
    void onTemperatureStatusStateChanged(const TemperatureStatusState& state) final;
    void onTemperatureSampleStateChanged(const TemperatureSampleState& state) final;
    void onFermentationStatusStateChanged(const FermentationStatusState& state) final;

// ==== GENERATED END: interface ==================================================

};

} // namespace Garbox