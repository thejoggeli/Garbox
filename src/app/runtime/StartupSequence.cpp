#include "StartupSequence.h"

#include "app/providers/PartsProvider.h"
#include "core/time/Time.h"
#include "core/util/function/default/EasingFunctions.h"
#include "modules/parts/button/InterruptButton.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"

namespace Garbox {

StartupSequence::StartupSequence():
    // initialize members
    mPiezoPlayer(PartsProvider::GetPiezoPlayer()),
    mStatusLeds(PartsProvider::GetStatusLeds()),
    mHeartbeatLed(mStatusLeds.getLed(static_cast<uint8_t>(StatusLedId::Heartbeat))){
    // nothing to do
}

void StartupSequence::run(){
    // sweep in
    playSweepIn();
    waitUntilSweepComplete();

    // sweep ount
    playSweepOut();
    waitUntilSweepComplete();
}

void StartupSequence::playSweepIn(){
    // play sweep in
    mPiezoPlayer.playTone(Tone(250_ms).sweep(300, 1000).duty(0.25f));

    // fade debug leds in
    for(AnimatedLed& led : mStatusLeds.getAllLeds()){
        led.setBrightness(0);
        led.setAnimation(EasingFunctions::GetInOutSine(), 1, 250_ms, 0.0f, 1.0f);
    }
}

void StartupSequence::playSweepOut(){
    // piezo play sweep out
    mPiezoPlayer.playTone(Tone(250_ms).sweep(1000, 300).duty(0.25f));

    // fade debug leds out
    for(AnimatedLed& led : mStatusLeds.getAllLeds()){
        led.setAnimation(EasingFunctions::GetInOutSine(), 1, 250_ms, 1.0f, 0.0f);
    }
}

void StartupSequence::waitUntilSweepComplete(){
    // wait until piezo + led animation complete
    while(mPiezoPlayer.isPlaying() || mHeartbeatLed.isAnimationPlaying()){
        Time::DelayMillis(1);
    }
}

} // namespace