// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "I2cPartsControllerAbs.h"

namespace Garbox {

I2cPartsControllerAbs::I2cPartsControllerAbs() : ControllerAbs(ComponentId::I2cPartsController, ControllerId::I2cParts){
    // nothing to do
}

I2cPartsControllerAbs::States& I2cPartsControllerAbs::states(){
    return mStates.value();
}

void I2cPartsControllerAbs::bindStates(
    TemperatureStatusState& temperatureStatus,
    TemperatureSampleState& temperatureSample
){
    mStates.emplace(
        temperatureStatus,
        temperatureSample
    );
}

} // namespace Garbox