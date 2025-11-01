#pragma once

#include "driver/spi_master.h"

namespace Garbox {

/**
 * SpiDma - asynchronous SPI DMA wrapper for ESP32-S3.
 */
class SpiDma {
public:

    struct Config {
        spi_host_device_t hostDevice;
        int32_t pinMosi;
        int32_t pinMiso;
        int32_t pinClk;
        int32_t pinCs;
        int32_t frequencyHz;
        int32_t maxTransferSize; 
        int32_t queueSize;
    };

    explicit SpiDma();

    void setup(Config const& config);
    void queue(const uint8_t* data, size_t lenBits, void* user = nullptr);
    bool poll(void (**callback)(void*), void** user);
    void setCallback(void (*cb)(void*));

private:

    spi_host_device_t mHost;
    spi_device_handle_t mDevice = nullptr;
    void (*mDoneCb)(void*) = nullptr;

    bool mInitialized = false;
};

} // namespace