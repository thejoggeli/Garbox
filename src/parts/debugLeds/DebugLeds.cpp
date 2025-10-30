#include "DebugLeds.h"

#include <array>
#include "assert/Assert.h"
#include "driver/rmt_tx.h"
#include "global/PinConfig.h"
#include "global/ledc/LedcInstances.h"
#include "led_strip.h"


namespace Garbox {

// Debug LEDs
static constexpr size_t NumDebugLeds = 4;
static std::array<Garbox::LedcChannel*, NumDebugLeds> sLeds = {
    &LedcInstances::GetDebugLed0Channel(),
    &LedcInstances::GetDebugLed1Channel(),
    &LedcInstances::GetDebugLed2Channel(),
    &LedcInstances::GetDebugLed3Channel()
};

// RGB LED (using ESP-IDF led_strip driver)
static led_strip_handle_t gRgbStrip = nullptr;

// initialized flag
static bool gInitialized = false;

void DebugLeds::Init(){
    
    if (gInitialized) {
        AssertExit(false, "DebugLeds::Init()", "already initialized");
        return;
    }

    // --- Initialize RMT channel for LED ---
    rmt_tx_channel_config_t tx_chan_config = {
        .gpio_num = static_cast<gpio_num_t>(PinConfig::RgbLed),
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10'000'000, // 10 MHz resolution (100 ns ticks)
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
        .intr_priority = 0,
        .flags = {}
    };

    rmt_channel_handle_t rmt_chan = nullptr;
    esp_err_t err = rmt_new_tx_channel(&tx_chan_config, &rmt_chan);
    if (err != ESP_OK) {
        AssertExit(false, "DebugLeds::Init()", "rmt_new_tx_channel failed");
        return;
    }

    // --- LED strip configuration ---
    led_strip_config_t strip_config = {
        .strip_gpio_num = PinConfig::RgbLed,
        .max_leds = 1,
        .led_model = LED_MODEL_WS2812,
        .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
        .flags = {}
    };

    // --- RMT-specific LED configuration ---
    led_strip_rmt_config_t rmt_strip_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10'000'000,
        .mem_block_symbols = 64,
        .flags = {
            .with_dma = 0
        }
    };

    err = led_strip_new_rmt_device(&strip_config, &rmt_strip_config, &gRgbStrip);
    if (err != ESP_OK) {
        AssertExit(false, "DebugLeds::Init()", "led_strip_new_rmt_device failed");
        return;
    }

    gInitialized = true;
}

void DebugLeds::ToggleLed(Id id, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DebugLeds::ToggleLed()", "not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        AssertDebug(false, "DebugLeds::ToggleLed()", "invalid id");
        return;
    }

    // toggle led
    if(sLeds[index]->getDuty() == 0){
        sLeds[index]->setDutyRelative(brightness); // set debug led to brightness
    }
    else {
        sLeds[index]->setDutyRaw(0); // turn debug led off
    }
}

void DebugLeds::SetLed(Id id, bool enable, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DeubgLeds::SetLed()", "not initialized");
        return;
    }

    // check if valid id
    size_t const index = static_cast<size_t>(id);
    if(index >= sLeds.size()){
        AssertDebug(false, "DebugLeds::SetLed()", "invalid id");
        return;
    }

    // set led
    if(enable){
        sLeds[index]->setDutyRelative(brightness); // set debug led to brightness
    }
    else {
        sLeds[index]->setDutyRaw(0); // turn debug led off
    }
}

void DebugLeds::SetAllLeds(bool enable, float brightness){

    // check if initialized
    if(!gInitialized){
        AssertDebug(false, "DebugLeds::SetAllLeds()", "not initialized");
        return;
    }

    // set leds
    if(enable){
        // set all debug leds to brightness
        for(LedcChannel* led : sLeds){
            led->setDutyRelative(brightness);
        }
    } 
    else {
        // turn all debug leds off
        for(LedcChannel* led : sLeds){
            led->setDutyRaw(0);
        }
    }
}

void DebugLeds::SetRgbLed(uint8_t r, uint8_t g, uint8_t b) {

    // check if initialized
    if (!gInitialized) {
        AssertDebug(false, "DebugLeds::SetRgbLed()", "not initialized");
        return;
    }

    // set pixel color internally
    esp_err_t err = led_strip_set_pixel(gRgbStrip, 0, r, g, b);
    if (err != ESP_OK) {
        AssertDebug(false, "DebugLeds::SetRgbLed()", "led_strip_set_pixel failed");
        return;
    }

    // write pixel colors to strip
    err = led_strip_refresh(gRgbStrip);
    if (err != ESP_OK) {
        AssertDebug(false, "DebugLeds::SetRgbLed()", "led_strip_refresh failed");
    }
}

}