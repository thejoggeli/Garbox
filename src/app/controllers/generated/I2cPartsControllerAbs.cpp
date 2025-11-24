// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "I2cPartsControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

I2cPartsControllerAbs::I2cPartsControllerAbs():
    // init memberes
    ControllerAbs(ComponentId::I2cPartsController, ControllerId::I2cParts){
    // nothing to do
}

TemperatureStatusEvent I2cPartsControllerAbs::makeTemperatureStatusEvent(){
    return ControllerAbs::makeEvent<EventType::TemperatureStatus>();
}

TemperatureSampleEvent I2cPartsControllerAbs::makeTemperatureSampleEvent(){
    return ControllerAbs::makeEvent<EventType::TemperatureSample>();
}

void I2cPartsControllerAbs::sendEvent(const TemperatureStatusEvent& event){
    publishEvent(event.header());
}

void I2cPartsControllerAbs::sendEvent(const TemperatureSampleEvent& event){
    publishEvent(event.header());
}

} // namespace Garbox