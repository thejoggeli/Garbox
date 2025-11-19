#pragma once

#include <cstdint>
#include <cstddef>
#include <functional>
#include "core/hardware/spi/SpiDma.h"

namespace Garbox {

class SpiDmaChunkSender {
public:
    using CompleteHandler = std::function<void(bool success)>;

    SpiDmaChunkSender(SpiDma& spi, size_t maxChunkBytes);
    ~SpiDmaChunkSender();

    void setCompleteHandler(CompleteHandler handler);

    void start(const uint8_t* data, size_t totalBytes);
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
