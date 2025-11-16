#pragma once

#include <cstddef>
#include <stdint.h>

#include "assert/Assert.h"

namespace Garbox {

template<typename T>
class RingBufferHeap {
public:

    RingBufferHeap(size_t capacity):
        // initialize members
        mCapacity(capacity) {
        // nothing to do
    }

    ~RingBufferHeap() {
        TriggerExit("RingBufferHeap", "heap using classes must not be deconstructed");
    }

    void init() {
        AssertExit(!mInitialized, "RingBufferHeap", "already initialized");

        mBuffer = new T[mCapacity];
        AssertExit(mBuffer != nullptr, "RingBufferHeap", "heap allocation failed");

        mHead = 0;
        mTail = 0;
        mCount = 0;
        mInitialized = true;
    }

    void clear() {
        if(!mInitialized) {
            TriggerDebug("RingBufferHeap", "not initialized");
            return;
        }

        mHead = 0;
        mTail = 0;
        mCount = 0;
    }

    bool push(const T& item) {
        if(isFull()) {
            return false;
        }

        mBuffer[mHead] = item;
        advance(mHead);
        ++mCount;

        return true;
    }

    T* pushPtr() {
        if(isFull()) {
            return nullptr;
        }

        T* slot = &mBuffer[mHead];
        advance(mHead);
        ++mCount;

        return slot;
    }

    bool pop(T& item) {
        if(isEmpty()) {
            return false;
        }

        item = mBuffer[mTail];
        advance(mTail);
        --mCount;

        return true;
    }

    T* popPtr() {
        if(isEmpty()) {
            return nullptr;
        }

        T* item = &mBuffer[mTail];
        advance(mTail);
        --mCount;

        return item;
    }

    bool peek(T& item) const {
        if(isEmpty()) {
            return false;
        }

        item = mBuffer[mTail];
        return true;
    }

    T* peekPtr() {
        if(isEmpty()) {
            return nullptr;
        }

        return &mBuffer[mTail];
    }

    bool isEmpty() const {
        return (mCount == 0);
    }

    bool isFull() const {
        return (mCount == mCapacity);
    }

    size_t size() const {
        return mCount;
    }

    size_t capacity() const {
        return mCapacity;
    }

    size_t available() const {
        return mCapacity - mCount;
    }

private:

    void advance(size_t& index) {
        if(++index == mCapacity) {
            index = 0;
        }
    }

    size_t mCapacity;
    T* mBuffer = nullptr;
    size_t mHead = 0;
    size_t mTail = 0;
    size_t mCount = 0;
    bool mInitialized = false;
};

} // namespace Garbox
