// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "generated/states/types/TemperatureStatusState.h"
#include "generated/states/types/TemperatureSampleState.h"

namespace Garbox {

class I2cPartsControllerStates final {
public:

    I2cPartsControllerStates(
        TemperatureStatusState& temperatureStatusState, // write
        TemperatureSampleState& temperatureSampleState // write
    );

    // disallow copy and move
    I2cPartsControllerStates(const I2cPartsControllerStates&) = delete;
    I2cPartsControllerStates& operator=(const I2cPartsControllerStates&) = delete;
    I2cPartsControllerStates(I2cPartsControllerStates&&) = delete;
    I2cPartsControllerStates& operator=(I2cPartsControllerStates&&) = delete;

    // writable states
    TemperatureStatusState& temperatureStatus;
    TemperatureSampleState& temperatureSample;

};

} // namespace