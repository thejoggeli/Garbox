#include "SpiDmaChunkSender.h"

#include "core/assert/Assert.h"
#include "core/log/Log.h"

namespace Garbox {

namespace {

// trampoline from SpiDma callback into the SpiDmaChunkSender instance
void SpiDmaChunkSenderDmaCallback(void* user, bool success){
    AssertExit(user != nullptr, "SpiDmaChunkSender", "dma callback user is nullptr");
    static_cast<SpiDmaChunkSender*>(user)->handleDmaComplete(success);
}

} // anonymous namespace

SpiDmaChunkSender::SpiDmaChunkSender(SpiDma& spi, size_t maxChunkBytes):
    mSpi(spi),
    mData(nullptr),
    mTotalBytes(0),
    mSentBytes(0),
    mMaxChunkBytes(maxChunkBytes),
    mUser(nullptr),
    mHandler(nullptr),
    mBusy(false){
    AssertExit(mMaxChunkBytes > 0, "SpiDmaChunkSender", "maxChunkBytes must be > 0");
}

SpiDmaChunkSender::~SpiDmaChunkSender(){
    TriggerExit("SpiDmaChunkSender", "std::function can use heap");
}

void SpiDmaChunkSender::setCompleteHandler(CompleteHandler handler){
    mHandler = handler;
}

void SpiDmaChunkSender::start(const uint8_t* data, size_t totalBytes){

    AssertExit(!mBusy, "SpiDmaChunkSender", "start() called while busy");
    AssertExit(data != nullptr, "SpiDmaChunkSender", "data is nullptr");
    AssertExit(totalBytes > 0, "SpiDmaChunkSender", "totalBytes must be > 0");

    mData       = data;
    mTotalBytes = totalBytes;
    mSentBytes  = 0;
    mBusy       = true;

    // kick off first chunk
    sendNext();
}

void SpiDmaChunkSender::handleDmaComplete(bool success){

    if(!mBusy){
        TriggerDebug("SpiDmaChunkSender", "onDmaComplete() while not busy");
        return;
    }

    if(!success){
        // abort sequence and notify user
        mBusy = false;
        if(mHandler){
            mHandler(false);
        }
        return;
    }

    // previous chunk finished, send next or finish
    if(mSentBytes >= mTotalBytes){
        // all bytes sent
        mBusy = false;
        if(mHandler){
            mHandler(true);
        }
        return;
    }

    // send next chunk
    sendNext();
}

bool SpiDmaChunkSender::isBusy() const {
    return mBusy;
}

void SpiDmaChunkSender::sendNext(){

    AssertExit(mBusy, "SpiDmaChunkSender", "sendNext() called while not busy");

    const size_t remaining = mTotalBytes - mSentBytes;
    if(remaining == 0){
        // nothing left to send, onDmaComplete() will handle completion
        return;
    }

    const size_t chunkBytes = (remaining > mMaxChunkBytes) ? mMaxChunkBytes : remaining;
    const uint8_t* chunkPtr = mData + mSentBytes;

    // queue async DMA transfer
    const bool ok = mSpi.transferAsync(chunkPtr, chunkBytes * 8, this, SpiDmaChunkSenderDmaCallback);

    if(!ok){
        TriggerDebug("SpiDmaChunkSender", "transferAsync() failed");
        mBusy = false;
        if(mHandler){
            mHandler(false);
        }
        return;
    }

    mSentBytes += chunkBytes;
}

} // namespace Garbox
