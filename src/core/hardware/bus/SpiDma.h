#pragma once

#include <cstdint>
#include <cstddef>
#include <cstring>
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace Garbox {

/**
 * Allocates some heap memory on initialization for transaction queue
 */
class SpiDma {
public:

    using TxCallback = void (*)(void* user);

    struct Config {
        spi_host_device_t hostDevice;
        int32_t pinMosi = -1;
        int32_t pinMiso = -1;
        int32_t pinClk = -1;
        int32_t pinCs = -1;
        int32_t frequencyHz = 40'000'000;
        int32_t maxTransferSize = 1024;
        int32_t queueSize = 3;
        UBaseType_t txCompleteTaskPriority = 5;
        uint32_t txCompleteTaskStackSize = 4096;
    };

    explicit SpiDma();
    ~SpiDma();

    void setup(const Config& config);
    void queue(const uint8_t* data, size_t lenBits, void* user = nullptr, TxCallback callback = nullptr);
    void setTxCallback(TxCallback callback);

private:
    struct TxSlot {
        spi_transaction_t trans;
        TxCallback callback = nullptr;
        bool inUse = false;
    };

    spi_host_device_t mHost;
    spi_device_handle_t mDevice = nullptr;
    TxCallback mTxCallback = nullptr;
    bool mInitialized = false;

    TxSlot* mTxSlots = nullptr;
    int mNumSlots = 0;

    TaskHandle_t mCompletionTask = nullptr;

    TxSlot* allocFreeSlot();
    void freeSlot(TxSlot* slot);

    static void completionTaskTrampoline(void* arg);
    void completionTask();
    void handleCompletedTransaction(spi_transaction_t* transaction);
    void invokeCallback(TxSlot* slot, void* user);
};

} // namespace Garbox
