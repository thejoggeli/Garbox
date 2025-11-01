#include "SpiDma.h"

#include "assert/Assert.h"

namespace Garbox {

SpiDma::SpiDma(){
    // nothing to do
}

void SpiDma::setup(Config const& config) {

    AssertExit(!mInitialized, "SpiDma::setup()", "already initialized");

    mHost = config.hostDevice;

    spi_bus_config_t busCfg = {};
    busCfg.mosi_io_num = config.pinMosi;
    busCfg.miso_io_num = config.pinMiso;
    busCfg.sclk_io_num = config.pinClk;
    busCfg.quadwp_io_num = -1;
    busCfg.quadhd_io_num = -1;
    busCfg.max_transfer_sz = config.maxTransferSize;

    spi_device_interface_config_t devCfg = {};
    devCfg.mode = 0;
    devCfg.clock_speed_hz = config.frequencyHz;
    devCfg.spics_io_num = config.pinCs;
    devCfg.queue_size = config.queueSize;

    if (spi_bus_initialize(mHost, &busCfg, SPI_DMA_CH_AUTO) != ESP_OK){
        AssertExit(false, "SpiDma::setup()", "spi_bus_initialize failed");
    }

    if (spi_bus_add_device(mHost, &devCfg, &mDevice) != ESP_OK){
        AssertExit(false, "SpiDma::setup()", "spi_bus_add_device failed");
    }

    mInitialized = true;
}

void SpiDma::queue(const uint8_t* data, size_t lenBits, void* user) {
    spi_transaction_t* t = (spi_transaction_t*)heap_caps_calloc(1, sizeof(spi_transaction_t), MALLOC_CAP_DMA);
    if (!t) return;

    t->tx_buffer = data;
    t->length = lenBits;
    t->user = user;

    spi_device_queue_trans(mDevice, t, portMAX_DELAY);
}

bool SpiDma::poll(void (**callback)(void*), void** user) {
    spi_transaction_t* t;
    esp_err_t ret = spi_device_get_trans_result(mDevice, &t, 0); // non-blocking
    if (ret == ESP_OK && t){
        if (user){
            *user = t->user;
        }
        if (callback && mDoneCb){
            *callback = mDoneCb;
        }
        heap_caps_free(t);
        return true;
    }
    return false;
}

void SpiDma::setCallback(void (*cb)(void*)) {
    mDoneCb = cb;
}

} // namespace
