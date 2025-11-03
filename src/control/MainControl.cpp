#include "MainControl.h"

#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/piezo/PiezoSequences.h"
#include "parts/debugLeds/DebugLeds.h"
#include "util/color/ColorMap.h"
#include "util/color/Rgb888.h"

namespace Garbox {

MainControl::MainControl() : 
    // init memebers
    mFan(),
    mHeatpad(),
    mDisplay(),
    mPiezo(),
    mPiezoPlayer(mPiezo){
    // nothing to do
}

void MainControl::init(){
    AssertExit(!mInitialized, "MainControl::init()", "already initialized");
    mFan.init();
    mFan.setEnabled(0);
    mFan.setSpeed(0.0F);
    mHeatpad.init();
    mDisplay.init();
    mPiezo.init();
    mInitialized = true;
}

void MainControl::start(){
    mFan.start();
    mFanStateTimer.start(0);
    mHeatpad.setDutyCycle(0.5f);
    mHeartbeatTimer.start(HeartbeatInterval);
    mPiezoPlayer.playSequence(PiezoSequences::Startup, 500_ms);
    mPiezoPlayer.playSequence(PiezoSequences::Button, 500_ms);
}

void MainControl::tick(){

    // heartbeat 
    if(mHeartbeatTimer.isExpired()){
        DebugLeds::ToggleLed(DebugLeds::Id::Heartbeat);
        mHeartbeatTimer.restart();
    }

    // update fan state
    constexpr uint32_t numFanStates = 6;
    static uint32_t fanState = 0;
    if(mFanStateTimer.isExpired()){
        switch(fanState){
            case 0:
                mPiezoPlayer.playSequence(PiezoSequences::Helix);
                mFan.setEnabled(0);
                mFan.setSpeed(0.0f);
                mFanStateTimer.restart(4000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, false);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(0.4f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.4f);
                break;
            case 2:
                mFan.setEnabled(1);
                mFan.setSpeed(0.6f);
                mFanStateTimer.restart(8000_ms);
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
                mFanStateTimer.restart(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 1.0f);
                break;
            case 5:
                mFan.setEnabled(1);
                mFan.setSpeed(0.5f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::SetLed(DebugLeds::Id::Custom1, true, 0.5f);
                break;
            default:
                // nothing to do
                AssertDebug(false, "MainControl", "unhandled fan state");
                break;
        }
        LogDebug("MainControl", "Fan State: %" PRIu32, fanState);
        fanState = (fanState + 1) % numFanStates;
    }

    // fan tick
    mFan.tick();

    if(mRpmTimer.isExpired() || mRpmTimer.isReset()){
        // print fan rpm
        static float lastRpmValue = 0;
        float const rpmValue = mFan.getMeasuredRpm();
        float const rpmChange = std::abs(lastRpmValue - rpmValue);
        if(rpmChange > 5.0f){ 
            LogDebug("MainControl", "Measured RPM: %" PRIu32, static_cast<uint32_t>(rpmValue));
            lastRpmValue = rpmValue;
        }
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
    float const tColorMap = static_cast<float>(fanState) * (1.0f / static_cast<float>(numFanStates - 1));
    RgbFloat rgbFloat = RgbFloat::fromHsl(colorMap.interpolateHsl(tColorMap)) * brightness;
    Rgb888 rgb = Rgb888::fromFloat(rgbFloat);
    DebugLeds::SetRgbLed(rgb.r, rgb.g, rgb.b);

    // display tick
    mDisplay.tick();

    // piezo tick
    mPiezoPlayer.tick();
}

void MainControl::onAssertDebug(const char* context, const char* message){
    if(!mInitialized){
        return;
    }
}

void MainControl::onAssertExit(const char* context, const char* message){
    if(!mInitialized){
        return;
    }
}

}