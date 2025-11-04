#pragma once

#include "driver/spi_master.h"
#include "freertos/task.h"

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

    using TxCallback = void (*)(void* user, bool success);

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
        const char* txCompleteTaskName;
        UBaseType_t txCompleteTaskPriority = 5;
        uint32_t txCompleteTaskStackSize = 1024;
    };

    explicit SpiDma();
    ~SpiDma();
    
    void setup(const Config& config);
    void setTxCallback(TxCallback callback);

    bool transferSync(const uint8_t* data, size_t lenBits, void* user = nullptr);
    bool transferAsync(const uint8_t* data, size_t lenBits, void* user = nullptr, TxCallback callback = nullptr);

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

    portMUX_TYPE mSlotLock = portMUX_INITIALIZER_UNLOCKED;
    size_t mMaxTransferSizeBits = 0;

    bool validateTransferArgs(const uint8_t* data, size_t lenBits);

    TxSlot* allocFreeSlot();
    void freeSlot(TxSlot* slot);

    static void completionTaskTrampoline(void* arg);
    void completionTask();
    void handleCompletedTransaction(spi_transaction_t* transaction, bool success);
    void invokeCallback(TxSlot* slot, void* user, bool success);
};

} // namespace Garbox
