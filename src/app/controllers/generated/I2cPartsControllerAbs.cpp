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
    return makeEvent<EventType::TemperatureStatus>();
}

TemperatureSampleEvent I2cPartsControllerAbs::makeTemperatureSampleEvent(){
    return makeEvent<EventType::TemperatureSample>();
}

void I2cPartsControllerAbs::sendEvent(const TemperatureStatusEvent& event){
    sendEvent(event);
}

void I2cPartsControllerAbs::sendEvent(const TemperatureSampleEvent& event){
    sendEvent(event);
}

} // namespace Garbox