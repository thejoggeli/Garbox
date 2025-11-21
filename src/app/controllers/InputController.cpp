#include "InputController.h"

#include "app/providers/PartsProvider.h"
#include "app/providers/PiezoSequences.h"
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/time/Time.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"
#include "core/util/function/default/EasingFunctions.h"

namespace Garbox {

InputController::InputController(const RuntimeContext& context):
    // initialize members
    InputControllerAbs(context),
    mButtonStatusLed(PartsProvider::GetStatusLed(StatusLedId::Custom2)),
    mPiezoPlayer(PartsProvider::GetPiezoPlayer()),
    mButton(PartsProvider::GetEncoderButton()){
    // nothing to do
}

void InputController::onInit(){

    // button state changed
    mButton.setStateChangedCallback([this](ButtonState oldState, ButtonState newState, void* userData){
        handleButtonStateChanged(oldState, newState);
    });

    // button holding
    mButton.setHoldCallback([this](uint32_t counter, uint32_t holdTimeMicros, void* userData){
        handleButtonHold(counter, holdTimeMicros);
    });

}

void InputController::onStart(){
    // nothing to do
}

void InputController::onInputTick(){
    // button tick
    mButton.tick();
}

void InputController::handleButtonStateChanged(ButtonState oldState, ButtonState newState){
    LogDebug("InputController", "button state changed: %s => %s", ButtonStateToString(oldState), ButtonStateToString(newState));
    const uint32_t deadTime = 0;
    switch(newState){
    case ButtonState::Pressed: {
        mPiezoPlayer.playTone(Tone(40_ms, 2000), deadTime);
        mButtonStatusLed.setBrightness(1.0f);
        break;
    }
    case ButtonState::PressedLong:
        mPiezoPlayer.playTone(Tone(80_ms, 3000), deadTime);
        break;
    case ButtonState::Released: {
        mPiezoPlayer.playTone(Tone(80_ms, 1000), deadTime);
        mButtonStatusLed.setAnimation(EasingFunctions::GetOutSine(), 1, 125_ms, 1.0f, 0.0f);
        break;
    }
    default:
        // nothing to do
        break;
    }
}

void InputController::handleButtonHold(uint32_t counter, uint32_t holdTimeMicros){
    const uint32_t frequency = 300 + counter * 100;
    const uint32_t deadTime = 0;
    if(frequency > 3000){
        TriggerExit("InputController", "Testing");
    }
    else if(!mPiezoPlayer.isPlaying()){
        mPiezoPlayer.playTone(Tone(100_ms, frequency), deadTime);
    }
}

} // namespace