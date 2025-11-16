#include "PartsProvider.h"

#include "app/parts/StatusLeds.h"
#include "assert/Assert.h"
#include "global/hardware/adc/AdcInstances.h"
#include "global/hardware/gpio/GpioInstances.h"
#include "global/hardware/ledc/LedcInstances.h"
#include "global/hardware/spi/SpiInstances.h"
#include "global/hardware/timer/TimerInstances.h"
#include "parts/button/InterruptButton.h"
#include "parts/display/Display.h"
#include "parts/fan/Fan.h"
#include "parts/heatpad/Heatpad.h"
#include "parts/led/rgb/RgbLed.h"
#include "parts/led/single/AnimatedLed.h"
#include "parts/piezo/PiezoPlayer.h"
#include "util/container/Span.h"

namespace Garbox {

void PartsProvider::Init(){

    // init fan
    Fan& fan = GetFan();
    fan.init();

    // init headpad
    Heatpad& heatpad = GetHeatpad();
    heatpad.init();
    heatpad.setDutyCycle(0.5f);
    heatpad.setPeriodDurationMicros(5000_ms);

    // init display
    Display& display = GetDisplay();
    display.init();

    // init piezo player
    PiezoPlayer& piezoPlayer = GetPiezoPlayer();
    piezoPlayer.init();

    // init encoder button
    ButtonIfc& button = GetEncoderButton();
    button.init();
    button.setPressedToReleasedDelayMicros(1_ms);
    button.setReleasedToPressedDelayMicros(1_ms);
    button.setPressedHoldTimeMicros(10_ms);
    button.setReleasedHoldTimeMicros(40_ms);
    button.setLongPressMicros(600_ms),
    button.setInitialHoldDelayMicros(1200_ms);
    button.setRepeatHoldDelayMicros(300_ms);

    // init status leds
    StatusLeds& statusLeds = GetStatusLeds();
    statusLeds.init();

    // init rgb led
    RgbLed& rgbLed = GetRgbLed();
    rgbLed.init();
}

void PartsProvider::Tick(){

    // tick fan
    GetFan().tick();

    // tick headpad
    GetHeatpad().tick();

    // tick encoder button
    GetEncoderButton().tick();
    
}

Fan& PartsProvider::GetFan(){
    static Fan instance(
        GpioInstances::GetFanEnable(),
        LedcInstances::GetFanSpeedChannel(),
        GpioInstances::GetFanTacho(),
        TimerInstances::GetFanTacho()
    );
    return instance;
}

Heatpad& PartsProvider::GetHeatpad(){
    static Heatpad instance(
        GpioInstances::GetHeatEnable(),
        AdcInstances::GetHeatpadVoltage(),
        AdcInstances::GetHeatpadCurrent()
    );
    return instance;
}

Display& PartsProvider::GetDisplay(){
    static Display instance(
        SpiInstances::GetSpiDma(),
        GpioInstances::GetDisplayRst(),
        GpioInstances::GetDisplayDc(),
        LedcInstances::GetDisplayBacklightChannel()
    );
    return instance;
}

PiezoPlayer& PartsProvider::GetPiezoPlayer(){
    static PiezoPlayer instance(
        LedcInstances::GetPiezoTimer(),
        LedcInstances::GetPiezoChannel()
    );
    return instance;
}

ButtonIfc& PartsProvider::GetEncoderButton(){
    static InterruptButton instance(
        GpioInstances::GetEncoderButton()
    );
    return instance;
}

AnimatedLed& PartsProvider::GetStatusLed(StatusLedId id){
    return GetStatusLeds().getLed(id);
}

StatusLeds& PartsProvider::GetStatusLeds(){

    // animated leds
    static AnimatedLed leds[] = {
        LedcInstances::GetDebugLed0Channel(),
        LedcInstances::GetDebugLed1Channel(),
        LedcInstances::GetDebugLed2Channel(),
        LedcInstances::GetDebugLed3Channel(),
    };
    static Span span(leds);

    // status leds instance
    static StatusLeds instance(span);
    return instance;
}

RgbLed& PartsProvider::GetRgbLed(){
    static RgbLed instance(GpioInstances::GetRgbLed());
    return instance;
}

} // namespace