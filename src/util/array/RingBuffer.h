#pragma once

#include <cstdint>

template <typename T, std::size_t N>
class RingBuffer {
public:

    RingBuffer() : mHead(0), mTail(0), mCount(0) {
        // nothing to do
    }

    bool push(const T& item) {
        if (isFull()){
            return false;
        }
        mBuffer[mHead] = item;
        advance(mHead);
        ++mCount;
        return true;
    }

    bool pop(T& item) {
        if (isEmpty()){
            return false;
        }
        item = mBuffer[mTail];
        advance(mTail);
        --mCount;
        return true;
    }

    T* popPtr() {
        if (isEmpty()){
            return nullptr;
        }
        T* item = &mBuffer[mTail];
        advance(mTail);
        --mCount;
        return item;
    }

    bool peek(T& item) const {
        if (isEmpty()){
            return false;
        }
        item = mBuffer[mTail];
        return true;
    }

    bool isEmpty() const { 
        return mCount == 0; 
    }

    bool isFull() const { 
        return mCount == N; 
    }

    std::size_t size() const { 
        return mCount; 
    }

    std::size_t capacity() const { 
        return N; 
    }

    void clear() {
        mHead = 0;
        mTail = 0;
        mCount = 0;
    }

private:

    void advance(std::size_t& index) {
        if (++index == N){
            index = 0;
        }
    }

    T mBuffer[N];
    std::size_t mHead;
    std::size_t mTail;
    std::size_t mCount;
};
