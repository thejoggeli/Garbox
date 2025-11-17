#pragma once

#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "core/rtos/Task.h"

namespace Garbox {

/**
 * SPI DMA helper with queued async transfers and optional callbacks.
 *
 * NOTE:
 *  - Synchronous calls (transferSync) from within the completion callback
 *    are NOT supported for now. This could be added later using semaphores
 *    for safe reentry, but is intentionally disabled to keep behavior simple.
 */
class SpiDma {
public:

    using CompleteCallback = void (*)(void* user, bool success);

    struct Config {
        spi_host_device_t hostDevice;
        int32_t pinMosi = -1;
        int32_t pinMiso = -1;
        int32_t pinClk = -1;
        int32_t pinCs = -1;
        uint8_t mode = 0;
        int32_t frequencyHz = 40'000'000;
        int32_t maxTransferSizeBytes = 1024;
        int32_t queueSize = 3;
    };

    explicit SpiDma();
    ~SpiDma();
    
    void init(const Config& config);

    void startTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);
    void stopTask();
    TaskHandle_t getTaskHandle();

    void setCompleteCallback(CompleteCallback callback);

    bool transferSync(const uint8_t* data, size_t lenBits, void* user = nullptr);
    bool transferAsync(const uint8_t* data, size_t lenBits, void* user = nullptr, CompleteCallback callback = nullptr);

    // Disallow copy and move 
    SpiDma(const SpiDma&) = delete;
    SpiDma& operator=(const SpiDma&) = delete;
    SpiDma(SpiDma&&) = delete;
    SpiDma& operator=(SpiDma&&) = delete;

private:
    struct TxSlot {
        spi_transaction_t trans;
        CompleteCallback callback = nullptr;
        bool inUse = false;
    };

    spi_host_device_t mHost;
    spi_device_handle_t mDevice = nullptr;
    CompleteCallback mTxCallback = nullptr;
    bool mInitialized = false;

    TxSlot* mTxSlots = nullptr;
    int mNumSlots = 0;

    SemaphoreHandle_t mMutex = nullptr;
    
    Task mTask;

    size_t mMaxTransferSizeBits = 0;

    bool validateTransferArgs(const uint8_t* data, size_t lenBits);

    TxSlot* allocFreeSlot();
    void freeSlot(TxSlot* slot);

    static void taskTrampoline(void* arg);
    void handleTask();
    void handleCompletedTransaction(spi_transaction_t* transaction, bool success);
    void invokeCallback(TxSlot* slot, void* user, bool success);
};

} // namespace Garbox
