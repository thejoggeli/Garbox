#include "SpiDma.h"

#include <cstring>
#include <esp_heap_caps.h>
#include "core/assert/Assert.h"
#include "core/log/Log.h"
#include "core/util/threading/LockGuard.h"

namespace Garbox {

SpiDma::SpiDma(){
    // nothing to do
}

SpiDma::~SpiDma(){
    // TODO: stop completion task
    // TODO: drain transactions
    // TODO: remove device and free bus
    // TODO: free allocated memory
    TriggerExit("SpiDma", "not implemented");
}

void SpiDma::init(const Config& config){
    AssertExit(!mInitialized, "SpiDma", "already initialized");

    // init members
    mHost = config.hostDevice;
    mNumSlots = config.queueSize;
    mMaxTransferSizeBits = static_cast<size_t>(config.maxTransferSizeBytes) * 8;

    // allocate memory for tx slots
    mTxSlots = new TxSlot[mNumSlots];
    AssertExit(mTxSlots != nullptr, "SpiDma", "TxSlot allocation failed");

    // initialize tx slots
    for (int i = 0; i < mNumSlots; ++i){
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
    if (spi_bus_initialize(mHost, &busCfg, SPI_DMA_CH_AUTO) != ESP_OK){
        TriggerExit("SpiDma", "spi_bus_initialize failed");
    }

    // initialize device
    if (spi_bus_add_device(mHost, &devCfg, &mDevice) != ESP_OK){
        TriggerExit("SpiDma", "spi_bus_add_device failed");
    }

    // task handler
    mTask.setHandler([this](){
        this->handleTask();
    });

    // create mutex
    mMutex = xSemaphoreCreateRecursiveMutex();
    AssertExit(mMutex != nullptr, "PiezoPlayer", "mutex creation failed");  

    mInitialized = true;
}

bool SpiDma::validateTransferArgs(const uint8_t* data, size_t lenBits){
    if (!mInitialized){
        TriggerDebug("SpiDma", "not initialized");
        return false;
    }
    if (data == nullptr){
        TriggerDebug("SpiDma", "invalid data ptr");
        return false;
    }
    if (!esp_ptr_internal(data) || !esp_ptr_dma_capable(data)){
        TriggerDebug("SpiDma", "TX buffer not in internal DMA-capable memory");
        return false;
    }
    if (lenBits == 0){
        TriggerDebug("SpiDma", "len bits must be > 0");
        return false;
    }
    if (lenBits & 0x7){
        TriggerDebug("SpiDma", "len bits must be divisible by 8");
        return false;
    }
    if (lenBits > mMaxTransferSizeBits){
        TriggerDebug("SpiDma", "len bits exceed max transfer size");
        return false;
    }
    return true;
}

SpiDma::TxSlot* SpiDma::allocFreeSlot(){
    LockGuard lock(mMutex);
    // find next free tx slot
    for (int i = 0; i < mNumSlots; ++i){
        if (!mTxSlots[i].inUse){
            mTxSlots[i].inUse = true;
            return &mTxSlots[i];
        }
    }
    // no free tx slot was found
    TriggerDebug("SpiDma::allocFreeSlot()", "no free tx slot found");
    return nullptr;
}

void SpiDma::freeSlot(TxSlot* slot){
    LockGuard lock(mMutex);
    if (slot == nullptr){
        TriggerDebug("SpiDma", "invalid slot ptr");
        return;
    }
    // mark slot as free
    slot->inUse = false;
    slot->callback = nullptr;
}

bool SpiDma::transferSync(const uint8_t* data, size_t lenBits, void* user){
    LockGuard lock(mMutex);

    if (!validateTransferArgs(data, lenBits)){
        return false;
    }

    if (xTaskGetCurrentTaskHandle() == mTask.getHandle()){
        TriggerDebug("SpiDma", "transferSync() not supported from completion callback");
        return false;
    }

    esp_err_t res = spi_device_acquire_bus(mDevice, portMAX_DELAY);
    if (res != ESP_OK){
        TriggerDebug("SpiDma", "spi_device_acquire_bus failed", static_cast<int32_t>(res));
        return false;
    }

    spi_transaction_t transaction = {};
    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;

    res = spi_device_polling_transmit(mDevice, &transaction);

    spi_device_release_bus(mDevice);

    if (res != ESP_OK){
        TriggerDebug("SpiDma", "spi_device_polling_transmit failed", static_cast<int32_t>(res));
        return false;
    }

    return true;
}

bool SpiDma::transferAsync(const uint8_t* data, size_t lenBits, void* user, CompleteCallback callback){
    LockGuard lock(mMutex);

    if (!validateTransferArgs(data, lenBits)){
        return false;
    }

    TxSlot* slot = allocFreeSlot();
    if (slot == nullptr){
        TriggerDebug("SpiDma", "no free tx slots available");
        return false;
    }

    spi_transaction_t& transaction = slot->trans;
    std::memset(&transaction, 0, sizeof(spi_transaction_t));
    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;
    slot->callback = callback;

    esp_err_t res = spi_device_queue_trans(mDevice, &transaction, portMAX_DELAY);
    if (res != ESP_OK){
        TriggerDebug("SpiDma", "failed to queue transaction", static_cast<int32_t>(res));
        freeSlot(slot);
        return false;
    }

    return true;
}

void SpiDma::setCompleteCallback(CompleteCallback callback){
    LockGuard lock(mMutex);
    mTxCallback = callback;
}

void SpiDma::startTask(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t core){
    LockGuard lock(mMutex);
    
    // check state
    AssertExit(mInitialized, "SpiDma", "not initialized");

    // start task
    mTask.configure(name, stackSize, priority, core);
    mTask.start();
}

void SpiDma::stopTask(){
    LockGuard lock(mMutex);
    mTask.stop();
    // TODO stop all spi activity
}

void SpiDma::handleTask(){

    spi_transaction_t* transaction = nullptr;
    constexpr TickType_t waitTicks = pdMS_TO_TICKS(1000); // 1000ms timeout

    while (true){
        esp_err_t ret = spi_device_get_trans_result(mDevice, &transaction, waitTicks);

        if (ret == ESP_OK){
            if (transaction != nullptr){
                handleCompletedTransaction(transaction, true);
            }
            else {
                TriggerDebug("SpiDma", "got ESP_OK but nullptr transaction");
            }
        }
        else if (ret != ESP_ERR_TIMEOUT){
            TriggerDebug("SpiDma", "spi_device_get_trans_result error");
        }
    }
}

TaskHandle_t SpiDma::getTaskHandle() const {
    return mTask.getHandle();
}

void SpiDma::handleCompletedTransaction(spi_transaction_t* transaction, bool success){
    LockGuard lock(mMutex);
    // invoke callback and free transaction slot
    for (int i = 0; i < mNumSlots; ++i){
        if (&mTxSlots[i].trans == transaction){
            invokeCallback(&mTxSlots[i], transaction->user, success);
            freeSlot(&mTxSlots[i]);
            return;
        }
    }
}

void SpiDma::invokeCallback(TxSlot* slot, void* user, bool success){
    LockGuard lock(mMutex);
    CompleteCallback globalCb = mTxCallback;

    if (slot->callback != nullptr){
        slot->callback(user, success);
    }
    else if (globalCb != nullptr){
        globalCb(user, success);
    }
}

} // namespace Garbox
