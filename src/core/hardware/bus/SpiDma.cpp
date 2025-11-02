#include "SpiDma.h"
#include "assert/Assert.h"
#include <cstring>

namespace Garbox {

SpiDma::SpiDma(){
    // nothing to do
}

SpiDma::~SpiDma() {
    if (mTxSlots) {
        delete[] mTxSlots;
        mTxSlots = nullptr;
    }
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
    busCfg.max_transfer_sz = config.maxTransferSize;

    // device config
    spi_device_interface_config_t devCfg = {};
    devCfg.mode = 0;
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

    // init complete
    mInitialized = true;

    // start background completion task automatically
    xTaskCreate(completionTaskTrampoline,
        "SpiDmaDoneTask", config.txCompleteTaskStackSize, this, config.txCompleteTaskPriority, &mCompletionTask
    );
}

SpiDma::TxSlot* SpiDma::allocFreeSlot() {
    // find next free tx slot
    for (int i = 0; i < mNumSlots; ++i) {
        if (!mTxSlots[i].inUse) {
            mTxSlots[i].inUse = true;
            return &mTxSlots[i];
        }
    }
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
    slot->inUse = false;
    slot->callback = nullptr;
}

void SpiDma::queue(const uint8_t* data, size_t lenBits, void* user, TxCallback callback) {

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

    // get free tx slot
    TxSlot* slot = allocFreeSlot();
    if (slot == nullptr){
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
    if (ret != ESP_OK) {
        AssertDebug(false, "SpiDma::queue()", "failed to queue transaction");
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

        // check result
        if (ret != ESP_OK || transaction == nullptr) {
            AssertDebug(false, "SpiDma::completionTask()", "invalid result from spi_device_get_trans_result()");
            continue;
        }

        // handle transaction complete
        handleCompletedTransaction(transaction);
    }
}

void SpiDma::handleCompletedTransaction(spi_transaction_t* transaction){
    // invoke callback and free transaction slot
    for (int i = 0; i < mNumSlots; ++i) {
        if (&mTxSlots[i].trans == transaction) {
            invokeCallback(&mTxSlots[i], transaction->user);
            freeSlot(&mTxSlots[i]);
            break;
        }
    }
}

void SpiDma::invokeCallback(TxSlot* slot, void* user) {
    // per-call callback takes priority
    if (slot->callback != nullptr) {
        slot->callback(user);
    } 
    else if (mTxCallback != nullptr) {
        mTxCallback(user);
    }
}

} // namespace Garbox
