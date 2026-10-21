#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace Garbox {

class DisplayBufferTracker final {
public:
    static constexpr std::size_t Capacity = 3U;

    enum class State : uint8_t {
        Unknown,
        LvglOwned,
        Queued,
        DmaOwned,
    };

    enum class Result : uint8_t {
        Ok,
        NullBuffer,
        UnknownBuffer,
        DuplicateBuffer,
        CapacityExceeded,
        InvalidTransition,
        DmaBusy,
        OutOfOrder,
        WrongDmaCompletion,
    };

    Result registerBuffer(const void* buffer){
        if(buffer == nullptr){
            return Result::NullBuffer;
        }
        if(findEntry(buffer) != nullptr){
            return Result::DuplicateBuffer;
        }
        if(mRegisteredCount == Capacity){
            return Result::CapacityExceeded;
        }

        mEntries[mRegisteredCount] = Entry{buffer, State::LvglOwned};
        ++mRegisteredCount;
        return Result::Ok;
    }

    Result queue(const void* buffer){
        Entry* entry = findEntry(buffer);
        if(entry == nullptr){
            return buffer == nullptr ? Result::NullBuffer : Result::UnknownBuffer;
        }
        if(entry->state != State::LvglOwned || mQueueCount == Capacity){
            return Result::InvalidTransition;
        }

        const std::size_t tail = (mQueueHead + mQueueCount) % Capacity;
        mQueue[tail] = buffer;
        ++mQueueCount;
        entry->state = State::Queued;
        return Result::Ok;
    }

    Result cancelQueued(const void* buffer){
        Entry* entry = findEntry(buffer);
        if(entry == nullptr){
            return buffer == nullptr ? Result::NullBuffer : Result::UnknownBuffer;
        }
        if(entry->state != State::Queued || mQueueCount == 0U){
            return Result::InvalidTransition;
        }

        const std::size_t tail = (mQueueHead + mQueueCount - 1U) % Capacity;
        if(mQueue[tail] != buffer){
            return Result::OutOfOrder;
        }

        mQueue[tail] = nullptr;
        --mQueueCount;
        entry->state = State::LvglOwned;
        return Result::Ok;
    }

    Result beginDma(const void* buffer){
        Entry* entry = findEntry(buffer);
        if(entry == nullptr){
            return buffer == nullptr ? Result::NullBuffer : Result::UnknownBuffer;
        }
        if(entry->state != State::Queued || mQueueCount == 0U){
            return Result::InvalidTransition;
        }
        if(mDmaBuffer != nullptr){
            return Result::DmaBusy;
        }
        if(mQueue[mQueueHead] != buffer){
            return Result::OutOfOrder;
        }

        mQueue[mQueueHead] = nullptr;
        mQueueHead = (mQueueHead + 1U) % Capacity;
        --mQueueCount;
        mDmaBuffer = buffer;
        entry->state = State::DmaOwned;
        return Result::Ok;
    }

    Result completeDma(const void* buffer){
        Entry* entry = findEntry(buffer);
        if(entry == nullptr){
            return buffer == nullptr ? Result::NullBuffer : Result::UnknownBuffer;
        }
        if(mDmaBuffer == nullptr){
            return Result::InvalidTransition;
        }
        if(mDmaBuffer != buffer){
            return Result::WrongDmaCompletion;
        }
        if(entry->state != State::DmaOwned){
            return Result::InvalidTransition;
        }

        mDmaBuffer = nullptr;
        entry->state = State::LvglOwned;
        return Result::Ok;
    }

    State stateOf(const void* buffer) const {
        const Entry* entry = findEntry(buffer);
        return entry != nullptr ? entry->state : State::Unknown;
    }

    std::size_t registeredCount() const { return mRegisteredCount; }
    std::size_t queuedCount() const { return mQueueCount; }
    const void* dmaBuffer() const { return mDmaBuffer; }

private:
    struct Entry {
        const void* buffer = nullptr;
        State state = State::Unknown;
    };

    Entry* findEntry(const void* buffer){
        for(std::size_t i = 0; i < mRegisteredCount; ++i){
            if(mEntries[i].buffer == buffer){
                return &mEntries[i];
            }
        }
        return nullptr;
    }

    const Entry* findEntry(const void* buffer) const {
        for(std::size_t i = 0; i < mRegisteredCount; ++i){
            if(mEntries[i].buffer == buffer){
                return &mEntries[i];
            }
        }
        return nullptr;
    }

    std::array<Entry, Capacity> mEntries{};
    std::array<const void*, Capacity> mQueue{};
    std::size_t mRegisteredCount = 0U;
    std::size_t mQueueHead = 0U;
    std::size_t mQueueCount = 0U;
    const void* mDmaBuffer = nullptr;
};

} // namespace Garbox
