// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "TimeSeriesControllerAbs.h"

namespace Garbox {

TimeSeriesControllerAbs::TimeSeriesControllerAbs() : ControllerAbs(ComponentId::TimeSeriesController, ControllerId::TimeSeries){
    // nothing to do
}

TimeSeriesControllerStates& TimeSeriesControllerAbs::states(){
    return mStates.value();
}

} // namespace Garbox