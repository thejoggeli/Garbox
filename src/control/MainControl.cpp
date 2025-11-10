#include "MainControl.h"

#include "assert/Assert.h"
#include "assert/AssertHandler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "global/util/FunctionInstances.h"
#include "global/hardware/gpio/GpioInstances.h"
#include "global/piezo/PiezoSequences.h"
#include "parts/led/DebugLeds.h"
#include "util/color/ColorMap.h"
#include "util/color/Rgb888.h"
#include "util/math/MathUtils.h"

namespace Garbox {

static constexpr uint32_t HeartbeatInterval = 2000_ms;

MainControl::MainControl() : 
    // init memebers
    mHeartbeatLed(DebugLeds::GetLed(DebugLeds::Id::Heartbeat)),
    mFan(),
    mHeatpad(),
    mDisplay(),
    mPiezoPlayer(),
    mButton(GpioInstances::GetRotaryEncoderButton()){
    // nothing to do
}

void MainControl::init(){
    AssertExit(!mInitialized, "MainControl", "already initialized");

    // init piezo player and play sweep in
    mPiezoPlayer.init();
    mPiezoPlayer.playTone(Tone(250_ms).sweep(300, 1000).duty(0.25f));

    // fade debug leds in
    for(AnimatedLed* led : DebugLeds::GetAllLeds()){
        led->setBrightness(0);
        led->setAnimation(FunctionInstances::GetEaseInOutSineSampled(), 1, 250_ms, 0.0f, 1.0f);
    }

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
    mButton.setPressedToReleasedDelayMicros(1_ms);
    mButton.setReleasedToPressedDelayMicros(1_ms);
    mButton.setPressedHoldTimeMicros(10_ms);
    mButton.setReleasedHoldTimeMicros(40_ms);
    mButton.setLongPressMicros(600_ms),
    mButton.setInitialHoldDelayMicros(1200_ms);
    mButton.setRepeatHoldDelayMicros(300_ms);
    mButton.setStateChangedCallback([this](Button::State oldState, Button::State newState, void* userData){
        handleButtonStateChanged(oldState, newState);
    });
    mButton.setHoldCallback([this](uint32_t counter, uint32_t holdTimeMicros, void* userData){
        handleButtonHold(counter, holdTimeMicros);
    });

    // init headpad
    mHeatpad.init();
    mHeatpad.setDutyCycle(0.5f);
    mHeatpad.setPeriodDurationMicros(5000_ms);

    // init display
    mDisplay.init();

    // wait until led animation complete (fade in animation might still be ongoing)
    while(mHeartbeatLed.isAnimationPlaying()){
        Time::DelayMillis(1);
    }

    // wait until piezo player complete (sweep in might still be playing)
    while(mPiezoPlayer.isPlaying()){
        Time::DelayMillis(1);
    }

    // piezo play sweep out
    mPiezoPlayer.playTone(Tone(250_ms).sweep(1000, 300).duty(0.25f));

    // fade debug leds out
    for(AnimatedLed* led : DebugLeds::GetAllLeds()){
        led->setAnimation(FunctionInstances::GetEaseInOutSineSampled(), 1, 250_ms, 1.0f, 0.0f);
    }

    // wait until led fade out complete
    while(mHeartbeatLed.isAnimationPlaying()){
        Time::DelayMillis(1);
    }

    // wait until piezo play sweep out complete
    while(mPiezoPlayer.isPlaying()){
        Time::DelayMillis(1);
    }

    // setup heartbeat led animation
    mHeartbeatLed.animationClear();
    mHeartbeatLed.animationAddFrame(FunctionInstances::GetEaseInOutSineSampled(), 800_ms, 0.0f,  1.0f);
    mHeartbeatLed.animationAddDelay(200_ms);
    mHeartbeatLed.animationAddFrame(FunctionInstances::GetEaseInOutSineSampled(), 800_ms, 1.0f,  0.0f);

    // init complete
    mInitialized = true;
}

void MainControl::start(){

    // start fan start time
    mFanStateTimer.start(0);

    // start parts
    mFan.start();
    mHeatpad.start();

    // start heartbeat led animation
    mHeartbeatLed.animationStart();
    
    // start heartbeat timer
    mHeartbeatTimer.start(HeartbeatInterval);

}

void MainControl::tick(){

    if(mHeartbeatTimer.isExpired()){
        mHeartbeatLed.animationStart();
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
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(0.0f, 600_ms);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(0.4f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(0.4f, 600_ms);
                break;
            case 2:
                mFan.setEnabled(1);
                mFan.setSpeed(0.6f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(0.6f, 600_ms);
                break;
            case 3:
                mFan.setEnabled(1);
                mFan.setSpeed(0.8f);
                mFanStateTimer.start(8000_ms);
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(0.8f, 600_ms);
                break;
            case 4:
                mFan.setEnabled(1);
                mFan.setSpeed(1.0f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(1.0f, 600_ms);
                break;
            case 5:
                mFan.setEnabled(1);
                mFan.setSpeed(0.5f);
                mFanStateTimer.restart(8000_ms);
                DebugLeds::GetLed(DebugLeds::Id::Custom1).setBrightnessSmooth(0.5f, 600_ms);
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
        float const rpmValue = mFan.getMeasuredRpm();
        float const rpmDiff = std::fabs(rpmValue - lastRpmValue);
        if(rpmDiff > 0.0f){ 
            LogDebug("MainControl", "Measured RPM: %.0f", rpmValue);
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
    static uint32_t periodMicros = 5000_ms;
    static float duty = 0.5f;
    switch(newState){
        case Button::State::Pressed:
            mPiezoPlayer.playTone(Tone(40_ms, 2000), deadTime);
            break;
        case Button::State::PressedLong:
            mPiezoPlayer.playTone(Tone(80_ms, 3000), deadTime);
            // update heatpad duty on long press
            duty = MathUtils::Wrap(duty + 0.25f, 0.25f, 1.0f);
            mHeatpad.setDutyCycle(duty);
            LogDebug("MainControl", "Heatpad set to: pwm=%2.0f%%, period=%u" PRIu32 "ms", 
                mHeatpad.getNextDutyCycle()*100.0f, 
                mHeatpad.getNextPeriodDurationMicros()/1000
            );
            break;
        case Button::State::Released:
            mPiezoPlayer.playTone(Tone(80_ms, 1000), deadTime);
            // update heatpad period on click
            if(oldState == Button::State::Pressed){
                periodMicros = MathUtils::Wrap(periodMicros + 1000_ms, 1000_ms, 8000_ms);
                mHeatpad.setPeriodDurationMicros(periodMicros);
                LogDebug("MainControl", "Heatpad set to: pwm=%2.0f%%, period=%" PRIu32 "ms", 
                    mHeatpad.getNextDutyCycle()*100.0f, 
                    mHeatpad.getNextPeriodDurationMicros()/1000
                );
            }
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
        // mPiezoPlayer.playSequence(PiezoSequences::GetFanEnabled());
    }
    else if(newState == Fan::State::Disabled){
        // mPiezoPlayer.playSequence(PiezoSequences::GetFanDisabled());
    }
}

void MainControl::handleFanStalledAlert(uint32_t counter){
    mPiezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

} // namespace
