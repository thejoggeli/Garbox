#pragma once
#include <cstdint>
#include <cstddef>
#include "core/hardware/bus/SpiDma.h"

namespace Garbox {

class SpiDmaChunkSender {
public:
    using CompleteHandler = void(*)(void* user, bool success);

    SpiDmaChunkSender(SpiDma& spi, size_t maxChunkBytes);

    void start(const uint8_t* data, size_t totalBytes, void* user, CompleteHandler handler);
    void handleDmaComplete(bool success);
    bool isBusy() const;

private:
    void sendNext();

    SpiDma& mSpi;
    const uint8_t* mData;
    size_t mTotalBytes;
    size_t mSentBytes;
    size_t mMaxChunkBytes;
    void* mUser;
    CompleteHandler mHandler;
    bool mBusy;
};

} // namespace Garbox
