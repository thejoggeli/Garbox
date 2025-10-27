#pragma once

#include <cstdint>

namespace Garbox {

class GpioConfig {
public:

    // Debug Leds
    static constexpr uint32_t DebugLed0 = 47;           // DEBUG_LED_0
    static constexpr uint32_t DebugLed1 = 21;           // DEBUG_LED_1
    static constexpr uint32_t DebugLed2 = 40;           // DEBUG_LED_2
    static constexpr uint32_t DebugLed3 = 39;           // DEBUG_LED_3

    // Debug GPIOs
    static constexpr uint32_t DebugGpio0 = 44;          // DEBUG_GPIO_0
    static constexpr uint32_t DebugGpio1 = 43;          // DEBUG_GPIO_1
    static constexpr uint32_t DebugGpio2 = 17;          // DEBUG_GPIO_2
    static constexpr uint32_t DebugGpio3 = 18;          // DEBUG_GPIO_3

    // Fan
    static constexpr uint32_t FanEnable = 4;            // FAN_ENABLE
    static constexpr uint32_t FanTacho = 5;             // FAN_TACHO
    static constexpr uint32_t FanPwm = 14;              // FAN_PWM

    // Display
    static constexpr uint32_t DisplaySda = 11;          // SPI_MOSI
    static constexpr uint32_t DisplayScl = 12;          // SPI_CLK
    static constexpr uint32_t DisplayCs = 10;           // SPI_CS0
    static constexpr uint32_t DisplayDc = 15;           // DISP_DC
    static constexpr uint32_t DisplayRst = 16;          // DISP_RST
    static constexpr uint32_t DisplayBlk = 38;          // DISP_BLK

    // Sensors (Temperature, Humidity, Brightness)
    static constexpr uint32_t SensorSda = 6;            // SENS_SDA
    static constexpr uint32_t SensorScl = 7;            // SENS_SCL

    // Piezo
    static constexpr uint32_t Piezo = 41;               // PIEZO

    // Rotary Encoder
    static constexpr uint32_t RotaryEncoderA = 2;       // ROTENC_A
    static constexpr uint32_t RotaryEncoderB = 42;      // ROTENC_B
    static constexpr uint32_t RotaryEncoderButton = 1;  // ROTENC_BTN

    // Heatpad
    static constexpr uint32_t HeatpadEnable = 13;       // HEAT_ENABLE
    static constexpr uint32_t HeatpadVoltageSense = 8;  // HEAT_USENSE
    static constexpr uint32_t HeatpadCurrentSense = 9;  // HEAT_ISENSE

};

} // namespace