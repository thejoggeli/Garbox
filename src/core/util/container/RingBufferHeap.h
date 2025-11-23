#pragma once

#include <cstddef>
#include <stdint.h>

#include "core/assert/Assert.h"

namespace Garbox {

template<typename T>
class RingBufferHeap {
public:

    RingBufferHeap(size_t capacity) : mCapacity(capacity) {
        mCapacity = capacity;
        mBuffer = new T[mCapacity];
        AssertExit(mBuffer != nullptr, "RingBufferHeap", "heap allocation failed");
    }

    ~RingBufferHeap(){
        AssertExit(mCapacity == 0, "RingBufferHeap", "heap using classes must not be deconstructed");
    }

    void clear() {
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

    size_t mCapacity = 0;
    T* mBuffer = nullptr;
    size_t mHead = 0;
    size_t mTail = 0;
    size_t mCount = 0;
};

} // namespace Garbox
