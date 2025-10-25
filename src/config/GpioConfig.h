#pragma once

#include <cstdint>

namespace Garbox {

class GpioConfig {
public:

    // Debug Leds
    static constexpr uint32_t DEBUG_LED_0 = 7;
    static constexpr uint32_t DEBUG_LED_1 = 6;
    static constexpr uint32_t DEBUG_LED_2 = 41;
    static constexpr uint32_t DEBUG_LED_3 = 40;

    // Debug GPIOs
    static constexpr uint32_t DEBUG_GPIO_0 = 21;
    static constexpr uint32_t DEBUG_GPIO_1 = 47;
    static constexpr uint32_t DEBUG_GPIO_2 = 17;
    static constexpr uint32_t DEBUG_GPIO_3 = 18;

    // Fan
    static constexpr uint32_t FAN_ENABLE = 4;
    static constexpr uint32_t FAN_TACHO = 5;
    static constexpr uint32_t FAN_PWM = 14;

    // Display
    static constexpr uint32_t DISP_SDA = 11; // SPI MOSI
    static constexpr uint32_t DISP_SCL = 12; // SPI CLK
    static constexpr uint32_t DISP_CS = 10; // SPI CS0
    static constexpr uint32_t DISP_DC = 15;
    static constexpr uint32_t DISP_RST = 16;
    static constexpr uint32_t DISP_BLK = 38;

    // Sensors (Temperature, Humidity, Brightness)
    static constexpr uint32_t SENS_SCL = 9;  
    static constexpr uint32_t SENS_SDA = 8;

    // Piezo
    static constexpr uint32_t PIEZO = 39;

    // Rotary Encoder
    static constexpr uint32_t ROTENC_A = 2;
    static constexpr uint32_t ROTENC_B = 42;
    static constexpr uint32_t ROTENC_BTN = 1;

    // Heatpad
    static constexpr uint32_t HEATPAD_ENABLE = 13;

};

} // namespace