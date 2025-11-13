#pragma once

#include <cstdint>

namespace Garbox {

class PinConfig {
public:

    // RGB Led
    static constexpr int32_t RgbLed = 48;              // Built-in RGB LED on ESP32-S3-DevkitC-1-U

    // Debug Leds
    static constexpr int32_t DebugLed0 = 47;           // DEBUG_LED_0
    static constexpr int32_t DebugLed1 = 21;           // DEBUG_LED_1
    static constexpr int32_t DebugLed2 = 40;           // DEBUG_LED_2
    static constexpr int32_t DebugLed3 = 39;           // DEBUG_LED_3

    // Debug GPIO
    static constexpr int32_t DebugGpio0 = 41;         // DEBUG_GPIO
    static constexpr int32_t DebugGpio1 = 15;         // PIEZO_B

    // Fan
    static constexpr int32_t FanEnable = 4;            // FAN_ENABLE
    static constexpr int32_t FanTacho = 5;             // FAN_TACHO
    static constexpr int32_t FanPwm = 13;              // FAN_PWM

    // Display
    static constexpr int32_t DisplaySda = 11;          // SPI_MOSI
    static constexpr int32_t DisplayScl = 12;          // SPI_CLK
    static constexpr int32_t DisplayCs = 17;           // SPI_CS0
    static constexpr int32_t DisplayDc = 18;           // DISP_DC
    static constexpr int32_t DisplayRst = 10;          // DISP_RST
    static constexpr int32_t DisplayBlk = 16;          // DISP_BLK

    // Sensors (Temperature, Humidity, Brightness)
    static constexpr int32_t SensorSda = 6;            // SENS_SDA
    static constexpr int32_t SensorScl = 7;            // SENS_SCL

    // Piezo
    static constexpr int32_t Piezo = 38;               // PIEZO

    // Rotary Encoder
    static constexpr int32_t RotaryEncoderA = 2;       // ROTENC_A
    static constexpr int32_t RotaryEncoderB = 42;      // ROTENC_B
    static constexpr int32_t RotaryEncoderC = 1;       // ROTENC_BTN

    // Heatpad
    static constexpr int32_t HeatpadEnable = 14;       // HEAT_ENABLE
    static constexpr int32_t HeatpadVoltageSense = 8;  // HEAT_USENSE
    static constexpr int32_t HeatpadCurrentSense = 9;  // HEAT_ISENSE

};

} // namespace