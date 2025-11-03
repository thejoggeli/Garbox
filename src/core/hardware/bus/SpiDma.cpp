#include "SpiDma.h"

#include <cstring>
#include "assert/Assert.h"

namespace Garbox {

portMUX_TYPE mSlotLock = portMUX_INITIALIZER_UNLOCKED;

SpiDma::SpiDma(){
    // nothing to do
}

SpiDma::~SpiDma() {
    AssertExit(false, "SpiDma::~SpiDma()", "not implemented");
}

void SpiDma::setup(const Config& config) {
    AssertExit(!mInitialized, "SpiDma::setup()", "already initialized");

    // init members
    mHost = config.hostDevice;
    mNumSlots = config.queueSize;

    // allocate memory for tx slots
    mTxSlots = new TxSlot[mNumSlots];
    AssertExit(mTxSlots != nullptr, "SpiDma::setup()", "TxSlot allocation failed");

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
        AssertExit(false, "SpiDma::setup()", "spi_bus_initialize failed");
    }

    // initialize device
    if (spi_bus_add_device(mHost, &devCfg, &mDevice) != ESP_OK) {
        AssertExit(false, "SpiDma::setup()", "spi_bus_add_device failed");
    }

    // counting semaphore: start "full" (all slots available)
    mAsyncSem = xSemaphoreCreateCounting(mNumSlots, mNumSlots);
    AssertExit(mAsyncSem != nullptr, "SpiDma::setup()", "failed to create async sem");

    // binary semaphore for sync exclusion
    mSyncSem = xSemaphoreCreateBinary();
    AssertExit(mSyncSem != nullptr, "SpiDma::setup()", "failed to create sync sem");
    xSemaphoreGive(mSyncSem); // set it "unlocked"

    // init complete
    mInitialized = true;

    // start background completion task automatically
    xTaskCreate(completionTaskTrampoline,
        "SpiDmaDoneTask", config.txCompleteTaskStackSize, this, config.txCompleteTaskPriority, &mCompletionTask
    );
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
    AssertDebug(false, "SpiDma::allocFreeSlot()", "no free tx slot found");
    return nullptr;
}

void SpiDma::freeSlot(TxSlot* slot) {
    if(slot == nullptr){
        AssertDebug(false, "SpiDma::freeSlot()", "invalid slot ptr");
        return;
    }
    // mark slot as free
    portENTER_CRITICAL(&mSlotLock);
    slot->inUse = false;
    slot->callback = nullptr;
    portEXIT_CRITICAL(&mSlotLock);
}

void SpiDma::transferSync(const uint8_t* data, size_t lenBits, void* user) {

    // check initialized
    if (!mInitialized){
        AssertDebug(false, "SpiDma::transferSync()", "not initialized");
        return;
    }

    // check data ptr
    if (data == nullptr){
        AssertDebug(false, "SpiDma::transferSync()", "invalid data ptr");
        return;
    }

    // check len bits
    if (lenBits == 0){
        AssertDebug(false, "SpiDma::transferSync()", "len bits must be > 0");
        return;
    }

    // get exclusive access
    xSemaphoreTake(mSyncSem, portMAX_DELAY);

    // wait until all async slots are released
    for (int i = 0; i < mNumSlots; ++i) {
        xSemaphoreTake(mAsyncSem, portMAX_DELAY);
    }

    spi_transaction_t transaction;
    std::memset(&transaction, 0, sizeof(transaction));

    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;

    // perform blocking transfer
    esp_err_t ret = spi_device_polling_transmit(mDevice, &transaction);

    // release async semaphores 
    for (int i = 0; i < mNumSlots; ++i) {
        xSemaphoreGive(mAsyncSem);
    }

    // release sync semaphore
    xSemaphoreGive(mSyncSem);

    // check result
    if (ret != ESP_OK) {
        AssertDebug(false, "SpiDma::transferSync()", "spi_device_polling_transmit failed");
    }
}

void SpiDma::transferAsync(const uint8_t* data, size_t lenBits, void* user, TxCallback callback) {

    // check initialized
    if (!mInitialized){
        AssertDebug(false, "SpiDma::queue()", "not initialized");
        return;
    }

    // check data ptr
    if (data == nullptr){
        AssertDebug(false, "SpiDma::queue()", "invalid data ptr");
        return;
    }

    // check len bits
    if (lenBits == 0){
        AssertDebug(false, "SpiDma::queue()", "len bits must be > 0");
        return;
    }

    // wait for sync lock to ensure no sync transfer is running
    xSemaphoreTake(mSyncSem, portMAX_DELAY);

    // lock an async slot
    xSemaphoreTake(mAsyncSem, portMAX_DELAY);

    // get free tx slot
    TxSlot* slot = allocFreeSlot();
    if (slot == nullptr){
        xSemaphoreGive(mAsyncSem);
        xSemaphoreGive(mSyncSem);
        AssertDebug(false, "SpiDma::queue()", "no free tx slots available");
        return;
    }

    // setup spi transaction 
    spi_transaction_t& transaction = slot->trans;
    std::memset(&transaction, 0, sizeof(spi_transaction_t));
    transaction.tx_buffer = data;
    transaction.length = lenBits;
    transaction.user = user;
    slot->callback = callback;

    // queue transaction
    esp_err_t ret = spi_device_queue_trans(mDevice, &transaction, portMAX_DELAY);

    // release sync semaphore again
    xSemaphoreGive(mSyncSem);  

    if (ret != ESP_OK) {
        AssertDebug(false, "SpiDma::queue()", "failed to queue transaction");
        xSemaphoreGive(mAsyncSem); // release again
        freeSlot(slot);
    }
}

void SpiDma::setTxCallback(TxCallback callback) {
    mTxCallback = callback;
}

void SpiDma::completionTaskTrampoline(void* arg) {
    static_cast<SpiDma*>(arg)->completionTask();
}

void SpiDma::completionTask() {
    spi_transaction_t* transaction = nullptr;
    while (true) {
        
        // Wait for the next completed SPI transaction.
        // This call blocks the task and yields to the FreeRTOS scheduler internally.
        esp_err_t ret = spi_device_get_trans_result(mDevice, &transaction, portMAX_DELAY);

        if (ret != ESP_OK) {
            AssertDebug(false, "SpiDma::completionTask()", "SPI transaction failed");
        }

        if (transaction != nullptr) {
            bool success = (ret == ESP_OK);
            handleCompletedTransaction(transaction, success);
            xSemaphoreGive(mAsyncSem); // always give back once per transaction
        }
        else {
            AssertDebug(false, "SpiDma::completionTask()", "received nullptr");
        }
    }
}

void SpiDma::handleCompletedTransaction(spi_transaction_t* transaction, bool success){
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
    // per-call callback takes priority
    if (slot->callback != nullptr) {
        slot->callback(user, success);
    } 
    else if (mTxCallback != nullptr) {
        mTxCallback(user, success);
    }
}

} // namespace Garbox
