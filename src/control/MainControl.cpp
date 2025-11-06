#include "MainControl.h"

#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/function/FunctionInstances.h"
#include "global/gpio/GpioInstances.h"
#include "global/piezo/PiezoSequences.h"
#include "parts/led/DebugLeds.h"
#include "util/color/ColorMap.h"
#include "util/color/Rgb888.h"
#include "util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t HeartbeatInterval = 2000_ms;

MainControl::MainControl() : 
    // init memebers
    mFan(),
    mHeatpad(),
    mDisplay(),
    mPiezoPlayer(),
    mButton(GpioInstances::GetRotaryEncoderButton()){
    // nothing to do
}

void MainControl::init(){
    AssertExit(!mInitialized, "MainControl", "already initialized");

    // init fan
    mFan.init();
    mFan.setStateChangedCallback([this](Fan::State oldState, Fan::State newState){
        handleFanStateChanged(oldState, newState);
    });
    mFan.setStalledAlertCallback([this](uint32_t counter){
        handleFanStalledAlert(counter);
    });

    // init button
    mButton.init();
    mButton.setPressDebounceMicros(5_ms);
    mButton.setReleaseDebounceMicros(40_ms);
    mButton.setLongPressMicros(400_ms),
    mButton.setInitialHoldDelayMicros(800_ms);
    mButton.setRepeatHoldDelayMicros(200_ms);
    mButton.setStateChangedCallback([this](Button::State oldState, Button::State newState, void* userData){
        handleButtonStateChanged(oldState, newState);
    });
    mButton.setHoldCallback([this](uint32_t counter, uint32_t holdTimeMicros, void* userData){
        handleButtonHold(counter, holdTimeMicros);
    });

    // init other
    mHeatpad.init();
    mDisplay.init();
    mPiezoPlayer.init();


    // init complete
    mInitialized = true;
}

void MainControl::start(){
    mFan.start();
    mFanStateTimer.start(0);
    mHeatpad.setDutyCycle(0.5f);
    mHeartbeatTimer.start(HeartbeatInterval);
}

void MainControl::tick(){

    if(mHeartbeatTimer.isExpired()){
        static const FunctionIfc& function = FunctionInstances::GetCosSampledNormNeg();
        static constexpr uint32_t numCycles = 1;
        static constexpr uint32_t durationMicros = HeartbeatInterval/2;
        DebugLeds::GetLed(DebugLeds::Id::Heartbeat).setPlayback(function, numCycles, durationMicros);
        mHeartbeatTimer.restart();
    }
    
    // button tick
    mButton.tick();

    // update fan state
    constexpr uint8_t numFanStates = 6;
    static uint8_t fanState = 0;
    if(mFanStateTimer.isExpired()){
        switch(fanState){
            case 0:
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
                TriggerDebug("MainControl", "unhandled fan state");
                break;
        }
        LogDebug("MainControl", "Fan State: %" PRIu32, fanState);
        fanState = MathUtils::Wrap<uint8_t>(fanState+1, numFanStates);
    }

    // fan tick
    mFan.tick();

    if(mRpmTimer.isExpired() || mRpmTimer.isReset()){
        // print fan rpm
        static float lastRpmValue = 0;
        uint32_t const rpmValue = mFan.getMeasuredRpm();
        uint32_t const rpmDiff = MathUtils::AbsDiff<uint32_t>(rpmValue, lastRpmValue);
        if(rpmDiff > 3){ 
            LogDebug("MainControl", "Measured RPM: %" PRIu32, rpmValue);
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

void MainControl::handleButtonStateChanged(Button::State oldState, Button::State newState){
    LogDebug("MainControl", "button state changed: %s => %s", Button::StateToString(oldState), Button::StateToString(newState));
    const uint32_t deadTime = 0;
    switch(newState){
        case Button::State::Pressed:
            mPiezoPlayer.playTone(Tone(80_ms, 2000), deadTime);
            break;
        case Button::State::PressedLong:
            mPiezoPlayer.playTone(Tone(80_ms, 3000), deadTime);
            break;
        case Button::State::Released:
            mPiezoPlayer.playTone(Tone(80_ms, 1000), deadTime);
            break;
        default:
            // nothing to do
            break;
    }
}

void MainControl::handleButtonHold(uint32_t counter, uint32_t holdTimeMicros){
    const uint32_t frequency = 300 + counter * 100;
    const uint32_t deadTime = 0;
    if(!mPiezoPlayer.isPlaying()){
        mPiezoPlayer.playTone(Tone(100_ms, frequency), deadTime);
    }
}

void MainControl::handleFanStateChanged(Fan::State oldState, Fan::State newState){
    LogDebug("MainControl", "fan state changed: %s => %s", 
        Fan::StateToString(oldState), 
        Fan::StateToString(newState)
    );
    if(newState == Fan::State::Enabled){
        mPiezoPlayer.playSequence(PiezoSequences::FanEnabled);
    }
    else if(newState == Fan::State::Disabled){
        mPiezoPlayer.playSequence(PiezoSequences::FanDisabled);
    }
}

void MainControl::handleFanStalledAlert(uint32_t counter){
    mPiezoPlayer.playSequence(PiezoSequences::FanStalled);
}

}