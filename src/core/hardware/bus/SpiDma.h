#pragma once
#include <Arduino.h>

extern "C" {
#include "driver/spi_master.h"
#include "esp_heap_caps.h"
#include "soc/spi_struct.h"
#include "soc/spi_reg.h"
}

/**
 * SpiDma - simple asynchronous SPI DMA wrapper for ESP32-S3 and compatible ESP32 variants.
 * 
 * Example usage:
 *   SpiDma spi(SPI2_HOST);
 *   spi.begin(23, 18, 5, 40000000);
 *   spi.queue(data, lenBits, userPtr);
 *   if (spi.poll(&cb, &user)) cb(user);
 */
class SpiDma {
public:
    explicit SpiDma(spi_host_device_t host = SPI2_HOST);

    bool begin(int mosi, int sclk, int cs, int freqHz = 40000000);
    void queue(const uint8_t* data, size_t lenBits, void* user = nullptr);
    bool poll(void (**callback)(void*), void** user);
    void setCallback(void (*cb)(void*));

private:
    spi_host_device_t mHost;
    spi_device_handle_t mDevice;
    void (*mDoneCb)(void*);
};
