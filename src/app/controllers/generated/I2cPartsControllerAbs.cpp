// *****************************************
// * THIS IS GENERATED CODE. DO NOT MODIFY *
// *****************************************

#include "I2cPartsControllerAbs.h"

#include "shared/types/ComponentId.h"

namespace Garbox {

I2cPartsControllerAbs::I2cPartsControllerAbs(const RuntimeContext& context): 
    // init members
    ControllerAbs(ComponentId::I2cPartsController, context){
    // nothing to do
}

I2cPartsControllerAbs::TemperatureStatusWrite I2cPartsControllerAbs::makeTemperatureStatusEvent(){
    return makeEvent<EventPayload::TemperatureStatus>();
}

I2cPartsControllerAbs::TemperatureSampleWrite I2cPartsControllerAbs::makeTemperatureSampleEvent(){
    return makeEvent<EventPayload::TemperatureSample>();
}

void I2cPartsControllerAbs::sendEvent(const TemperatureStatusWrite& event){
    sendEvent(event.header);
}

void I2cPartsControllerAbs::sendEvent(const TemperatureSampleWrite& event){
    sendEvent(event.header);
}

} // namespace Garbox