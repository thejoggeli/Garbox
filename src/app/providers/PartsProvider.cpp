#include "PartsProvider.h"

#include <algorithm>
#include "app/config/AppConfig.h"
#include "app/hardware/adc/AdcInstances.h"
#include "app/hardware/gpio/GpioInstances.h"
#include "app/hardware/ledc/LedcInstances.h"
#include "app/hardware/spi/SpiInstances.h"
#include "app/hardware/timer/TimerInstances.h"
#include "app/parts/StatusLeds.h"
#include "assert/Assert.h"
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
    static constexpr uint32_t tachoFilterTicks = AppConfig::TickFrequencyHz * 0.3f;
    static Fan instance(Fan::Config{
        .enableGpio = GpioInstances::GetFanEnable(),
        .speedPwm = LedcInstances::GetFanSpeedChannel(),
        .tachoGpio = GpioInstances::GetFanTacho(),
        .tachoTimer = TimerInstances::GetFanTacho(),
        .tachoPulsesPerRev = 2,
        .tachoFilterTicks = std::max(tachoFilterTicks, 1u)
    });
    return instance;
}

Heatpad& PartsProvider::GetHeatpad(){
    static constexpr uint32_t adcFilterTicks = AppConfig::TickFrequencyHz;
    static Heatpad instance(Heatpad::Config{
        .enableGpio = GpioInstances::GetHeatEnable(),
        .voltageSenseAdc = AdcInstances::GetHeatpadVoltage(),
        .currentSenseAdc = AdcInstances::GetHeatpadCurrent(),
        .adcFilterTicks = adcFilterTicks,
    });
    return instance;
}

Display& PartsProvider::GetDisplay(){
    static Display instance(Display::Config{
        .spi = SpiInstances::GetSpiDma(),
        .gpioRst = GpioInstances::GetDisplayRst(),
        .gpioDc = GpioInstances::GetDisplayDc(),
        .pwmBlk = LedcInstances::GetDisplayBacklightChannel(),
        .width = AppConfig::DisplayWidth,
        .height = AppConfig::DisplayHeight,
        .bytesPerPixel = AppConfig::DisplayBytesPerPixel,
        .bufferPartialFactor = AppConfig::DisplayPartialFactor,
        .bufferSizeBytes = AppConfig::DisplayBytesPerFlush,
        .bufferWidth = AppConfig::DisplayWidth,
        .bufferHeight = AppConfig::DisplayHeight / AppConfig::DisplayPartialFactor,
    });
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