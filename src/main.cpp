#include <Arduino.h>

#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "config/GpioConfig.h"
#include "core/Gpio.h"
#include "core/Time.h"
#include "fan/Fan.h"

using Garbox::AssertHandler;
using Garbox::Fan;
using Garbox::Gpio;
using Garbox::GpioConfig;
using Garbox::Time;

Fan fan;

void setup() {
    Serial.begin(115200); 

    // init global components
    AssertHandler::Init();
    Time::Init();

    // init other components
    fan.init();

    // start global components
    AssertHandler::Start();
    Time::Start();

    // start other components
    fan.start();
}

void loop() {

    // begin the tick
    Time::BeginTick();

    // update fan state
    static uint32_t fanState = 0;
    static uint32_t nextToggleTimeMillis = 0;
    if(Time::GetMillis() >= nextToggleTimeMillis){
        if(nextToggleTimeMillis != 0){
            if(++fanState > 2){
                fanState = 0;
            }
        }
        switch(fanState){
            case 0:
                fan.setEnabled(1);
                fan.setSpeed(0.0F);
                break;
            case 1:
                fan.setEnabled(1);
                fan.setSpeed(1.0F);
                break;
            case 2:
                fan.setEnabled(0);
                fan.setSpeed(0.0F);
                break;
            default:
                // nothing to do
                break;
        }
        Serial.println("Fan state: " + String(fanState));
        Serial.println("Fan measured RPM: " + String(fan.getMeasuredRpm()));
        nextToggleTimeMillis += 5000;
    }

    // fan tick
    fan.tick();

    // print fan rpm
    static uint32_t lastRpmValue = 0;
    uint32_t const rpmValue = fan.getMeasuredRpm();
    if(rpmValue != lastRpmValue){
        Serial.println("Fan measured RPM: " + String(rpmValue));
    }

    // end the tick
    Time::EndTick();
}