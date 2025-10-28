#include "MainControl.h"

#include <Arduino.h>
#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "color/ColorMap.h"
#include "color/Rgb888.h"
#include "config/DebugLedsConfig.h"
#include "core/Time.h"

namespace Garbox {

MainControl::MainControl() : 
    // init memebers
    mFan(),
    mHeatpad(),
    mDisplay(),
    mPiezo(){
    // nothing to do
}

void MainControl::init(){
    mFan.init();
    mFan.setEnabled(0);
    mFan.setSpeed(0.0F);
    mHeatpad.init();
    mDisplay.init();
    mPiezo.init();
}

void MainControl::start(){
    mFan.start();
    mFanStateTimer.reset();
    mRpmTimer.reset();
    mHeatpad.setDutyCycle(0.5f);
    mHeartbeatTimer.start(HeartbeatIntervalMillis);
}

void MainControl::tick(){

    // heartbeat 
    if(mHeartbeatTimer.isExpired()){
        DebugLeds::ToggleLed(DebugLedsConfig::Heartbeat);
        mHeartbeatTimer.restart();
    }

    // update fan state
    constexpr uint32_t numFatStates = 6;
    static uint32_t fanState = 0;
    if(mFanStateTimer.isExpired()){
        if(++fanState >= numFatStates){
            fanState = 0;
        }
        switch(fanState){
            case 0:
                mFan.setEnabled(0);
                mFan.setSpeed(0.0f);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, false);
                DebugLeds::SetLed(DebugLedsConfig::Custom2, false);
                mFanStateTimer.start(4000);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(0.4f);
                mFanStateTimer.start(8000);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, true, 0.4f);
                break;
            case 2:
                mFan.setEnabled(1);
                mFan.setSpeed(0.6f);
                mFanStateTimer.start(8000);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, true, 0.6f);
                break;
            case 3:
                mFan.setEnabled(1);
                mFan.setSpeed(0.8f);
                mFanStateTimer.start(8000);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, true, 0.8f);
                break;
            case 4:
                mFan.setEnabled(1);
                mFan.setSpeed(1.0f);
                mFanStateTimer.start(8000);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, true, 1.0f);
                break;
            case 5:
                mFan.setEnabled(1);
                mFan.setSpeed(0.5f);
                mFanStateTimer.start(8000);
                DebugLeds::SetLed(DebugLedsConfig::Custom1, true, 0.5f);
                break;
            default:
                // nothing to do
                break;
        }
        Serial.println("Fan state: " + String(fanState));
    }

    // fan tick
    mFan.tick();

    // print fan rpm
    static uint32_t lastRpmValue = 0;
    uint32_t const rpmValue = mFan.getMeasuredRpm();
    if((rpmValue != lastRpmValue) && mRpmTimer.isExpired()){
        Serial.println("Fan measured RPM: " + String(rpmValue));
        lastRpmValue = rpmValue;
        mRpmTimer.start(200);
    }

    // heatpd tick
    mHeatpad.tick();

    // rgb led tick
    static ColorMap const colorMap = {
        RgbFloat(0.0f, 1.0f, 0.0f),
        RgbFloat(1.0f, 0.0f, 0.0f),
    };
    constexpr uint8_t brightness = 5;
    float const tColorMap = static_cast<float>(fanState) * (1.0f / static_cast<float>(numFatStates - 1));
    Rgb888 rgb = Rgb888::fromHsl(colorMap.interpolateHsl(tColorMap));
    DebugLeds::SetRgbLed(rgb.r, rgb.g, rgb.b, brightness);

    // display tick
    mDisplay.tick();

    // piezo tick
    mPiezo.tick();
}

void MainControl::onAssertDebug(const char* message){
    DebugLeds::SetLed(DebugLedsConfig::Assert, true);
}

void MainControl::onAssertExit(const char* message){
    DebugLeds::SetAllLeds(true);
}

}