#include "PartsProvider.h"

#include <algorithm>

#include "app/config/AppConfig.h"
#include "app/hardware/AdcInstances.h"
#include "app/hardware/GpioInstances.h"
#include "app/hardware/I2cInstances.h"
#include "app/hardware/LedcInstances.h"
#include "app/hardware/SpiInstances.h"
#include "app/hardware/TimerInstances.h"

#include "core/assert/Assert.h"
#include "core/util/container/Span.h"

#include "modules/parts/button/InterruptButton.h"
#include "modules/parts/display/Display.h"
#include "modules/parts/fan/Fan.h"
#include "modules/parts/heatpad/Heatpad.h"
#include "modules/parts/led/rgb/RgbLed.h"
#include "modules/parts/led/single/AnimatedLed.h"
#include "modules/parts/led/AnimatedLedGroup.h"
#include "modules/parts/piezo/PiezoPlayer.h"
#include "modules/parts/temperature/Sht31.h"

namespace Garbox {

void PartsProvider::Init(){

    // init fan
    Fan& fan = GetFan();
    fan.init();

    // init headpad
    Heatpad& heatpad = GetHeatpad();
    heatpad.init();
    heatpad.setEnabled(false);
    heatpad.setDutyCycle(0.0f);
    heatpad.setPeriodDurationMicros(1000_ms);

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

    // init sht31
    Sht31& sht31 = GetTemperatureSensor();
    sht31.init({
        .address = 0x44,
        .mode = Sht31::PeriodicMode::Hz1,
    });

    // init status leds
    AnimatedLedGroup& statusLeds = GetStatusLeds();
    statusLeds.init();

    // init rgb led
    RgbLed& rgbLed = GetRgbLed();
    rgbLed.init();
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
        .enableGpio = GpioInstances::GetHeatpadEnable(),
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
        .gpioCs = GpioInstances::GetDisplayCs(),
        .pwmBlk = LedcInstances::GetBacklightChannel(),
        .width = AppConfig::DisplayWidth,
        .height = AppConfig::DisplayHeight,
        .bytesPerPixel = AppConfig::DisplayBytesPerPixel,
        .partialFactor = AppConfig::DisplayPartialFactor,
        .chunkSizeBytes = AppConfig::DisplayChunkSizeBytes,
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

Sht31& PartsProvider::GetTemperatureSensor(){
    static Sht31 instance(
        I2cInstances::GetI2c()
    );
    return instance;

}

AnimatedLed& PartsProvider::GetStatusLed(StatusLedId id){
    return GetStatusLeds().getLed(static_cast<uint8_t>(id));
}

AnimatedLedGroup& PartsProvider::GetStatusLeds(){

    // animated leds
    static AnimatedLed leds[] = {
        LedcInstances::GetStatusLed0Channel(),
        LedcInstances::GetStatusLed1Channel(),
        LedcInstances::GetStatusLed2Channel(),
        LedcInstances::GetStatusLed3Channel(),
    };
    static Span span(leds);

    // status leds instance
    static AnimatedLedGroup instance(span);
    return instance;
}

RgbLed& PartsProvider::GetRgbLed(){
    static RgbLed instance(GpioInstances::GetRgbLed());
    return instance;
}

LvglContext& PartsProvider::GetLvglContext(){
    return GetDisplay().getLvglHandler().getContext();
}

} // namespace