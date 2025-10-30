#include "MainControl.h"

#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "parts/debugLeds/DebugLeds.h"
#include "util/color/ColorMap.h"
#include "util/color/Rgb888.h"

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
    mFanStateTimer.start(1_s);
    mRpmTimer.reset();
    mHeatpad.setDutyCycle(0.5f);
    mHeartbeatTimer.start(HeartbeatInterval);
}

void MainControl::tick(){

    // heartbeat 
    if(mHeartbeatTimer.isExpired()){
        DebugLeds::ToggleLed(DebugLeds::Id::Heartbeat);
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
                DebugLeds::SetLed(DebugLeds::Id::Custom1, false);
                DebugLeds::SetLed(DebugLeds::Id::Custom2, false);
                mFanStateTimer.start(4000_ms);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(0.4f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.4f);
                break;
            case 2:
                mFan.setEnabled(1);
                mFan.setSpeed(0.6f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.6f);
                break;
            case 3:
                mFan.setEnabled(1);
                mFan.setSpeed(0.8f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.8f);
                break;
            case 4:
                mFan.setEnabled(1);
                mFan.setSpeed(1.0f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 1.0f);
                break;
            case 5:
                mFan.setEnabled(1);
                mFan.setSpeed(0.5f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.5f);
                break;
            default:
                // nothing to do
                break;
        }
        LogDebug("MainControl", "Fan State: %" PRIu32, fanState);
    }

    // fan tick
    mFan.tick();

    // print fan rpm
    static uint32_t lastRpmValue = 0;
    uint32_t const rpmValue = mFan.getMeasuredRpm();
    if((rpmValue != lastRpmValue) && mRpmTimer.isExpired()){
        LogDebug("MainControl", "Measured RPM: %" PRIu32, rpmValue);
        lastRpmValue = rpmValue;
        mRpmTimer.start(200_ms);
    }

    // heatpd tick
    mHeatpad.tick();

    // rgb led tick
    static ColorMap const colorMap = {
        RgbFloat(0.0f, 1.0f, 0.0f),
        RgbFloat(1.0f, 0.0f, 0.0f),
    };
    constexpr float brightness = 5.0f / 255.0f;
    float const tColorMap = static_cast<float>(fanState) * (1.0f / static_cast<float>(numFatStates - 1));
    RgbFloat rgbFloat = RgbFloat::fromHsl(colorMap.interpolateHsl(tColorMap)) * brightness;
    Rgb888 rgb = Rgb888::fromFloat(rgbFloat);
    DebugLeds::SetRgbLed(rgb.r, rgb.g, rgb.b);

    // display tick
    mDisplay.tick();

    // piezo tick
    mPiezo.tick();
}

void MainControl::onAssertDebug(const char* context, const char* message){
    DebugLeds::SetLed(DebugLeds::Id::Assert, true);
}

void MainControl::onAssertExit(const char* context, const char* message){
    DebugLeds::SetAllLeds(true);
}

}