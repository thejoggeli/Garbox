#pragma once

#include <cstddef>
#include <cstdint>

#include "core/assert/Assert.h"

namespace Garbox {

/**
 * DataPoolBase<Storage> is a simple bump allocator on top of a Storage
 * that provides raw byte memory and a fixed capacity in bytes.
 *
 * It is intended primarily for simple POD types. Allocations only move
 * an internal offset forward; releaseAll() just resets the offset without
 * calling destructors.
 */
template <typename Storage>
class DataPoolBase : private Storage {
public:

    // Construct with default Storage
    DataPoolBase() : Storage(), mOffsetBytes(0){}

    // Construct with arguments forwarded to Storage
    template<typename... Args>
    DataPoolBase(Args... storageArgs) : Storage(storageArgs...) , mOffsetBytes(0){}

    // Reset the allocation offset (does not destruct objects)
    void releaseAll(){
        mOffsetBytes = 0;
    }

    // Allocate a raw aligned memory block from the pool
    void* allocateRaw(std::size_t sizeBytes, std::size_t alignmentBytes){
        const std::size_t mask = alignmentBytes - 1;
        const std::size_t alignedOffset = (mOffsetBytes + mask) & ~mask;

        const bool enoughSpace = (alignedOffset + sizeBytes) <= Storage::capacityBytes();
        if(!enoughSpace){
            TriggerDebug("DataPool", "insufficient space");
            return 0;
        }

        uint8_t* buffer = reinterpret_cast<uint8_t*>(Storage::dataBytes());
        uint8_t* ptr = buffer + alignedOffset;

        mOffsetBytes = alignedOffset + sizeBytes;
        return ptr;
    }

    // Allocate memory for T without initializing it
    template<typename T>
    T* allocate(){
        return static_cast<T*>(allocateRaw(sizeof(T), alignof(T)));
    }

    // Allocate space and construct T in-place using value initialization.
    // Default member initializers are applied; otherwise POD members are zero-initialized.
    template<typename T>
    T* construct(){
        void* rawPtr = allocateRaw(sizeof(T), alignof(T));
        if(rawPtr == 0){
            return 0;
        }
        return new(rawPtr) T();
    }

    // Copy-construct T into storage using placement new
    template<typename T>
    T* push(const T& value){
        void* rawPtr = allocateRaw(sizeof(T), alignof(T));
        if(rawPtr == 0){
            return 0;
        }
        return new(rawPtr) T(value);
    }

    // Construct T in-place with arbitrary constructor arguments
    template<typename T, typename... Args>
    T* emplace(Args... args){
        void* rawPtr = allocateRaw(sizeof(T), alignof(T));
        if(rawPtr == 0){
            return 0;
        }
        return new(rawPtr) T(std::forward<Args>(args)...);
    }

    // Total capacity in bytes provided by the Storage
    std::size_t capacity() const {
        return Storage::capacityBytes();
    }

    // Number of bytes used so far
    std::size_t used() const {
        return mOffsetBytes;
    }

private:
    std::size_t mOffsetBytes;
};

} // namespace Garbox
