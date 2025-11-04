#include "SpiDma.h"

#include <cstring>
#include "assert/Assert.h"
#include "esp_heap_caps.h"

namespace Garbox {

SpiDma::SpiDma() {
    // nothing to do
}

SpiDma::~SpiDma() {
    // TODO: stop completion task
    // TODO: drain transactions
    // TODO: remove device and free bus
    // TODO: free allocated memory
    FailExit("SpiDma", "not implemented");
}

void SpiDma::setup(const Config& config) {
    AssertExit(!mInitialized, "SpiDma", "already initialized");

    // init members
    mHost = config.hostDevice;
    mNumSlots = config.queueSize;
    mMaxTransferSizeBits = static_cast<size_t>(config.maxTransferSizeBytes) * 8;

    // allocate memory for tx slots
    mTxSlots = new TxSlot[mNumSlots];
    AssertExit(mTxSlots != nullptr, "SpiDma", "TxSlot allocation failed");

    // initialize tx slots
    for (int i = 0; i < mNumSlots; ++i) {
        std::memset(&mTxSlots[i].trans, 0, sizeof(spi_transaction_t));
        mTxSlots[i].callback = nullptr;
        mTxSlots[i].inUse = false;
    }

    // bus config
    spi_bus_config_t busCfg = {};
    busCfg.mosi_io_num = config.pinMosi;
    busCfg.miso_io_num = config.pinMiso;
    busCfg.sclk_io_num = config.pinClk;
    busCfg.quadwp_io_num = -1;
    busCfg.quadhd_io_num = -1;
    busCfg.max_transfer_sz = config.maxTransferSizeBytes;

    // device config
    spi_device_interface_config_t devCfg = {};
    devCfg.mode = config.mode;
    devCfg.clock_speed_hz = config.frequencyHz;
    devCfg.spics_io_num = config.pinCs;
    devCfg.queue_size = config.queueSize;

    // initialize bus
    if (spi_bus_initialize(mHost, &busCfg, SPI_DMA_CH_AUTO) != ESP_OK) {
        FailExit("SpiDma", "spi_bus_initialize failed");
    }

    // initialize device
    if (spi_bus_add_device(mHost, &devCfg, &mDevice) != ESP_OK) {
        FailExit("SpiDma", "spi_bus_add_device failed");
    }

    mInitialized = true;

    // start background completion task automatically
    BaseType_t taskRes = xTaskCreate(
        completionTaskTrampoline,
        config.txCompleteTaskName,
        config.txCompleteTaskStackSize,
        this,
        config.txCompleteTaskPriority,
        &mCompletionTask
    );

    if (taskRes != pdPASS || mCompletionTask == nullptr) {
        FailExit("SpiDma", "failed to start completion task");
    }
}

bool SpiDma::validateTransferArgs(const uint8_t* data, size_t lenBits) {
    if (!mInitialized) {
        FailDebug("SpiDma", "not initialized");
        return false;
    }
    if (data == nullptr) {
        FailDebug("SpiDma", "invalid data ptr");
        return false;
    }
    if (!esp_ptr_internal(data) || !esp_ptr_dma_capable(data)) {
        FailDebug("SpiDma", "TX buffer not in internal DMA-capable memory");
        return false;
    }
    if (lenBits == 0) {
        FailDebug("SpiDma", "len bits must be > 0");
        return false;
    }
    if (lenBits & 0x7) {
        FailDebug("SpiDma", "len bits must be divisible by 8");
        return false;
    }
    if (lenBits > mMaxTransferSizeBits) {
        FailDebug("SpiDma", "len bits exceed max transfer size");
        return false;
    }
    return true;
}

SpiDma::TxSlot* SpiDma::allocFreeSlot() {
    // find next free tx slot
    portENTER_CRITICAL(&mSlotLock);
    for (int i = 0; i < mNumSlots; ++i) {
        if (!mTxSlots[i].inUse) {
            mTxSlots[i].inUse = true;
            portEXIT_CRITICAL(&mSlotLock);
            return &mTxSlots[i];
        }
    }
    portEXIT_CRITICAL(&mSlotLock);
    // no free tx slot was found
    FailDebug("SpiDma::allocFreeSlot()", "no free tx slot found");
    return nullptr;
}

void SpiDma::freeSlot(TxSlot* slot) {
    if (slot == nullptr) {
        FailDebug("SpiDma", "invalid slot ptr");
        return;
    }
    // mark slot as free
    portENTER_CRITICAL(&mSlotLock);
    slot->inUse = false;
    slot->callback = nullptr;
    portEXIT_CRITICAL(&mSlotLock);
}

bool SpiDma::transferSync(const uint8_t* data, size_t lenBits, void* user) {
    if (!validateTransferArgs(data, lenBits)) {
        return false;
    }

    if (xTaskGetCurrentTaskHandle() == mCompletionTask) {
        FailDebug("SpiDma", "transferSync() not supported from completion callback");
        return false;
    }

    esp_err_t res = spi_device_acquire_bus(mDevice, portMAX_DELAY);
    if (res != ESP_OK) {
        FailDebug("SpiDma", "spi_device_acquire_bus failed");
        return false;
    }

    spi_transaction_t transaction = {};
    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;

    res = spi_device_polling_transmit(mDevice, &transaction);

    spi_device_release_bus(mDevice);

    if (res != ESP_OK) {
        FailDebug("SpiDma", "spi_device_polling_transmit failed");
        return false;
    }

    return true;
}

bool SpiDma::transferAsync(const uint8_t* data, size_t lenBits, void* user, TxCallback callback) {
    if (!validateTransferArgs(data, lenBits)) {
        return false;
    }

    TxSlot* slot = allocFreeSlot();
    if (slot == nullptr) {
        FailDebug("SpiDma", "no free tx slots available");
        return false;
    }

    spi_transaction_t& transaction = slot->trans;
    std::memset(&transaction, 0, sizeof(spi_transaction_t));
    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;
    slot->callback = callback;

    esp_err_t ret = spi_device_queue_trans(mDevice, &transaction, portMAX_DELAY);
    if (ret != ESP_OK) {
        FailDebug("SpiDma", "failed to queue transaction");
        freeSlot(slot);
        return false;
    }

    return true;
}

void SpiDma::setTxCallback(TxCallback callback) {
    portENTER_CRITICAL(&mSlotLock);
    mTxCallback = callback;
    portEXIT_CRITICAL(&mSlotLock);
}

void SpiDma::completionTaskTrampoline(void* arg) {
    static_cast<SpiDma*>(arg)->completionTask();
}

void SpiDma::completionTask() {
    spi_transaction_t* transaction = nullptr;
    constexpr TickType_t waitTicks = pdMS_TO_TICKS(1000); // 1s timeout

    while (true) {
        esp_err_t ret = spi_device_get_trans_result(mDevice, &transaction, waitTicks);

        if (ret == ESP_OK) {
            if (transaction != nullptr) {
                handleCompletedTransaction(transaction, true);
            }
            else {
                FailDebug("SpiDma", "got ESP_OK but nullptr transaction");
            }
        }
        else if (ret == ESP_ERR_TIMEOUT) {
            // No completed transactions within 1s, normal if idle
            continue;
        }
        else {
            FailDebug("SpiDma", "spi_device_get_trans_result error");
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

void SpiDma::handleCompletedTransaction(spi_transaction_t* transaction, bool success) {
    // invoke callback and free transaction slot
    portENTER_CRITICAL(&mSlotLock);
    for (int i = 0; i < mNumSlots; ++i) {
        if (&mTxSlots[i].trans == transaction) {
            portEXIT_CRITICAL(&mSlotLock);
            invokeCallback(&mTxSlots[i], transaction->user, success);
            freeSlot(&mTxSlots[i]);
            return;
        }
    }
    portEXIT_CRITICAL(&mSlotLock);
}

void SpiDma::invokeCallback(TxSlot* slot, void* user, bool success) {
    portENTER_CRITICAL(&mSlotLock);
    TxCallback globalCb = mTxCallback;
    portEXIT_CRITICAL(&mSlotLock);

    if (slot->callback != nullptr) {
        slot->callback(user, success);
    }
    else if (globalCb != nullptr) {
        globalCb(user, success);
    }
}

} // namespace Garbox
