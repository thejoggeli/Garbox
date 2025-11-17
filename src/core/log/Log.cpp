#include "Log.h"
#include "assert/Assert.h"
#include "driver/uart.h"

namespace Garbox {

static bool mInitialized = false;

void Log::Init(){
    if (mInitialized){
        TriggerExit("Log", "already initialized");
        return;
    }

    // UART configuration
    uart_config_t uart_config = {
        .baud_rate = 921600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_APB
    };

    // apply the parameters
    uart_param_config(UART_NUM_0, &uart_config);

    // Install the driver with larger TX buffer (e.g. 2 KB)
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    // Set default log level
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    mInitialized = true;
}

void Log::SetLevel(Level level, const char* tag){
    esp_log_level_set(tag, static_cast<esp_log_level_t>(level));
}

} // namespace Garbox
