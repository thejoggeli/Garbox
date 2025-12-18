// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************
#include "I2cPartsControllerAbs.h"
#include "shared/types/ComponentId.h"

namespace Garbox {

I2cPartsControllerAbs::I2cPartsControllerAbs() : ControllerAbs(ComponentId::I2cPartsController, ControllerId::I2cParts){
    // nothing to do
}

TemperatureStatusEvent I2cPartsControllerAbs::makeTemperatureStatusEvent(){
    return ComponentAbs::makeEvent<EventType::TemperatureStatus>();
}

TemperatureSampleEvent I2cPartsControllerAbs::makeTemperatureSampleEvent(){
    return ComponentAbs::makeEvent<EventType::TemperatureSample>();
}

void I2cPartsControllerAbs::sendEvent(const TemperatureStatusEvent& event){
    sendEventToHost(event.header());
}

void I2cPartsControllerAbs::sendEvent(const TemperatureSampleEvent& event){
    sendEventToHost(event.header());
}
 
} // namespace Garbox