#include "GarboxController.h"

#include "app/parts/StatusLeds.h"
#include "app/providers/ColorMaps.h"
#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"

#include "assert/Assert.h"
#include "assert/AssertHandler.h"

#include "core/diagnostics/Profiler.h"
#include "core/log/Log.h"
#include "core/time/Time.h"

#include "parts/display/Display.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/led/rgb/RgbLed.h"
#include "parts/piezo/PiezoPlayer.h"

#include "util/color/ColorMap.h"
#include "util/color/types/Rgb888.h"
#include "util/function/default/EasingFunctions.h"
#include "util/math/MathUtils.h"

namespace Garbox {

GarboxController::GarboxController():
    // initialize members
    mRgbLed(PartsProvider::GetRgbLed()),
    mStatusLeds(PartsProvider::GetStatusLeds()),
    mFanStatusLed(mStatusLeds.getLed(StatusLedId::Custom1)),
    mButtonStatusLed(mStatusLeds.getLed(StatusLedId::Custom2)),
    mFan(PartsProvider::GetFan()),
    mHeatpad(PartsProvider::GetHeatpad()),
    mDisplay(PartsProvider::GetDisplay()),
    mPiezoPlayer(PartsProvider::GetPiezoPlayer()),
    mButton(PartsProvider::GetEncoderButton()){
    // nothing to do
}

void GarboxController::onInit(){

    // fan state changed
    mFan.setStateChangedCallback([this](Fan::State oldState, Fan::State newState){
        handleFanStateChanged(oldState, newState);
    });

    // fan stalled alert
    mFan.setStalledAlertCallback([this](uint32_t counter){
        handleFanStalledAlert(counter);
    });

    // button state changed
    mButton.setStateChangedCallback([this](ButtonState oldState, ButtonState newState, void* userData){
        handleButtonStateChanged(oldState, newState);
    });

    // button holding
    mButton.setHoldCallback([this](uint32_t counter, uint32_t holdTimeMicros, void* userData){
        handleButtonHold(counter, holdTimeMicros);
    });

}

void GarboxController::onStart(){
    // start fan start time
    mFanStateTimer.start(0);

    // start parts
    mFan.start();
    mHeatpad.start();
}

void GarboxController::onTick(){
        
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
                mFanStatusLed.setBrightnessSmooth(0.0f, 600_ms);
                break;
            case 1:
                mFan.setEnabled(1);
                mFan.setSpeed(0.4f);
                mFanStateTimer.restart(8000_ms);
                mFanStatusLed.setBrightnessSmooth(0.4f, 600_ms);
                break;
            case 2:
                mFan.setEnabled(1);
                mFan.setSpeed(0.6f);
                mFanStateTimer.restart(8000_ms);
                mFanStatusLed.setBrightnessSmooth(0.6f, 600_ms);
                break;
            case 3:
                mFan.setEnabled(1);
                mFan.setSpeed(0.8f);
                mFanStateTimer.start(8000_ms);
                mFanStatusLed.setBrightnessSmooth(0.8f, 600_ms);
                break;
            case 4:
                mFan.setEnabled(1);
                mFan.setSpeed(1.0f);
                mFanStateTimer.restart(8000_ms);
                mFanStatusLed.setBrightnessSmooth(1.0f, 600_ms);
                break;
            case 5:
                mFan.setEnabled(1);
                mFan.setSpeed(0.5f);
                mFanStateTimer.restart(8000_ms);
                mFanStatusLed.setBrightnessSmooth(0.5f, 600_ms);
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
    static const ColorMap& colorMap = ColorMaps::GetRedBlue();
    constexpr float brightness = 0.14f;
    float const tColorMap = mHeatpad.getMeasuredVoltage() / 17.0f;
    HslColor hslColor = colorMap.interpolateHsl(tColorMap);
    hslColor.l = brightness;
    mRgbLed.setColor(hslColor.toLinearRgb());
}

void GarboxController::onDisplayTick(){
    // check if display is ready to render the next frame
    if(mDisplay.isReady()){

        // lock display state
        mDisplay.takeLock();
        // TODO update widgets here
        mDisplay.giveLock();

        // notify display to start rendering the next frame
        mDisplay.notifyTask();
    }
}

void GarboxController::handleButtonStateChanged(ButtonState oldState, ButtonState newState){
    LogDebug("GarboxController", "button state changed: %s => %s", ButtonStateToString(oldState), ButtonStateToString(newState));
    const uint32_t deadTime = 0;
    static uint32_t periodMicros = 5000_ms;
    static float duty = 0.5f;
    switch(newState){
        case ButtonState::Pressed: {
            mPiezoPlayer.playTone(Tone(40_ms, 2000), deadTime);
            mButtonStatusLed.setBrightness(1.0f);
            break;
        }
        case ButtonState::PressedLong:
            mPiezoPlayer.playTone(Tone(80_ms, 3000), deadTime);
            // update heatpad duty on long press
            duty = MathUtils::Wrap(duty + 0.25f, 0.25f, 1.0f);
            mHeatpad.setDutyCycle(duty);
            LogDebug("GarboxController", "Heatpad set to: pwm=%2.0f%%, period=%u" PRIu32 "ms", 
                mHeatpad.getNextDutyCycle()*100.0f, 
                mHeatpad.getNextPeriodDurationMicros()/1000
            );
            break;
        case ButtonState::Released: {
            mPiezoPlayer.playTone(Tone(80_ms, 1000), deadTime);
            mButtonStatusLed.setAnimation(EasingFunctions::GetOutSine(), 1, 125_ms, 1.0f, 0.0f);
            // update heatpad period on click
            if(oldState == ButtonState::Pressed){
                periodMicros = MathUtils::Wrap(periodMicros + 1000_ms, 1000_ms, 8000_ms);
                mHeatpad.setPeriodDurationMicros(periodMicros);
                LogDebug("GarboxController", "Heatpad set to: pwm=%2.0f%%, period=%" PRIu32 "ms", 
                    mHeatpad.getNextDutyCycle()*100.0f, 
                    mHeatpad.getNextPeriodDurationMicros()/1000
                );
            }
            break;
        }
        default:
            // nothing to do
            break;
    }
}

void GarboxController::handleButtonHold(uint32_t counter, uint32_t holdTimeMicros){
    const uint32_t frequency = 300 + counter * 100;
    const uint32_t deadTime = 0;
    if(frequency > 3000){
        TriggerExit("GarboxController", "Testing");
    }
    else if(!mPiezoPlayer.isPlaying()){
        mPiezoPlayer.playTone(Tone(100_ms, frequency), deadTime);
    }
}

void GarboxController::handleFanStateChanged(Fan::State oldState, Fan::State newState){
    LogDebug("GarboxController", "fan state changed: %s => %s", 
        Fan::StateToString(oldState), 
        Fan::StateToString(newState)
    );
}

void GarboxController::handleFanStalledAlert(uint32_t counter){
    mPiezoPlayer.playSequence(PiezoSequences::GetFanStalled());
}

} // namespace