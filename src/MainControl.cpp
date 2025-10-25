#include "MainControl.h"

#include <Arduino.h>
#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "debug/DebugLeds.h"
#include "core/Time.h"

namespace Garbox {

MainControl::MainControl() : 
    // init memebers
    mFan(){
    // nothing to do
}

void MainControl::init(){
    mFan.init();
    mFan.setEnabled(0);
    mFan.setSpeed(0.0F);
}

void MainControl::start(){
    mFan.start();
    mFanStateTimer.start(FanStateIntervalMillis);
    mHeartbeatTimer.start(HeartbeatIntervalMillis);
}

void MainControl::tick(){

    // heartbeat 
    if(mHeartbeatTimer.isExpired()){
        DebugLeds::ToggleLed(DebugLeds::LedId::Heartbeat);
        mHeartbeatTimer.restart();
    }

    // update fan state
    static uint32_t fanState = 0;
    if(mFanStateTimer.isExpired()){
        if(++fanState > 2){
            fanState = 0;
        }
        switch(fanState){
            case 0:
                mFan.setEnabled(1);
                mFan.setSpeed(0.0F);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(1.0F);
                break;
            case 2:
                mFan.setEnabled(0);
                mFan.setSpeed(0.0F);
                break;
            default:
                // nothing to do
                break;
        }
        Serial.println("Fan state: " + String(fanState));
        Serial.println("Fan measured RPM: " + String(mFan.getMeasuredRpm()));
        mFanStateTimer.restart();
    }

    // fan tick
    mFan.tick();

    // print fan rpm
    static uint32_t lastRpmValue = 0;
    uint32_t const rpmValue = mFan.getMeasuredRpm();
    if(rpmValue != lastRpmValue){
        Serial.println("Fan measured RPM: " + String(rpmValue));
    }
}

void MainControl::onAssertDebug(const char* message){
    DebugLeds::SetLed(DebugLeds::LedId::AssertDebug, true);
}

void MainControl::onAssertExit(const char* message){
    DebugLeds::SetAllLeds(true);
}

}