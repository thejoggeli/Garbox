#include "Time.h"

#include <Arduino.h>
#include "config/GlobalConfig.h"

namespace Garbox {

uint32_t Time::sLastMicros64 = 0;
uint64_t Time::sOffsetMicros64 = 0;
uint32_t Time::sTickBeginTimeMicros = 0;

uint64_t Time::GetMicros64(){
    // get current micros (32 bit, possibly wrapped around) 
    uint32_t currentMicros = micros();

    // check if micros timer wrapped around (happens every 71 minutes)
    if(currentMicros < sLastMicros64){
        sOffsetMicros64 += 0x100000000;
    }

    // remember current value to detect overflow in next call
    sLastMicros64 = currentMicros;

    // return with offset
    return sOffsetMicros64 + currentMicros;
}

uint32_t Time::GetMicros(){
    return micros();
}

uint32_t Time::GetMillis(){
    return millis();
}

void Time::BeginTick(){
    // remember tick begin time
    sTickBeginTimeMicros = micros();

    // call each tick to make sure that no 32-bit wrap is missed
    GetMicros64(); 
}

void Time::EndTick(){
    // sleep for the remaining time to achieve the desired tick frequency
	uint32_t const passedTimeMicros = micros() - sTickBeginTimeMicros;
	if(passedTimeMicros < GlobalConfig::targetTickIntervalMicros){
        uint32_t const remainingTimeMicros = GlobalConfig::targetTickIntervalMicros - passedTimeMicros;
        delayMicroseconds(remainingTimeMicros);
    }
}

}