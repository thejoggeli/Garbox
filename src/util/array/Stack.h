#pragma once

#include <cstdint>
#include <cstddef>

namespace Garbox {

template <typename T, std::size_t N>
class Stack {
public:

    Stack() : mCount(0) {
        // nothing to do
    }

    bool push(const T& item) {
        if (isFull()){
            return false;
        }
        mBuffer[mCount++] = item;
        return true;
    }

    bool pop(T& item) {
        if (isEmpty()){
            return false;
        }
        item = mBuffer[--mCount];
        return true;
    }

    T* popPtr() {
        if (isEmpty()){
            return nullptr;
        }
        return &mBuffer[--mCount];
    }

    bool peek(T& item) const {
        if (isEmpty()){
            return false;
        }
        item = mBuffer[mCount - 1];
        return true;
    }

    const T* peekPtr() const {
        if (isEmpty()){
            return nullptr;
        }
        return &mBuffer[mCount - 1];
    }

    void clear() { 
        mCount = 0; 
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

private:
    T mBuffer[N];
    std::size_t mCount;
};

} // namespace
