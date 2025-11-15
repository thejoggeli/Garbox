#pragma once
#include <cstdint>
#include <cstddef>

namespace Garbox {

template <typename T>
class HeapRingBuffer {
public:
    explicit HeapRingBuffer(std::size_t capacity): 
        // init members
        mBuffer(new T[capacity]),
        mCapacity(capacity),
        mHead(0),
        mTail(0),
        mCount(0){
        // nothing to do
    }

    ~HeapRingBuffer(){
        delete[] mBuffer;
    }

    bool push(const T& item){
        if (isFull())
            return false;
        mBuffer[mHead] = item;
        advance(mHead);
        ++mCount;
        return true;
    }

    bool pop(T& item){
        if (isEmpty())
            return false;
        item = mBuffer[mTail];
        advance(mTail);
        --mCount;
        return true;
    }

    T* popPtr(){
        if (isEmpty())
            return nullptr;
        T* item = &mBuffer[mTail];
        advance(mTail);
        --mCount;
        return item;
    }

    bool peek(T& item) const {
        if (isEmpty())
            return false;
        item = mBuffer[mTail];
        return true;
    }

    bool isEmpty() const { 
        return mCount == 0; 
    }

    bool isFull() const { 
        return mCount == mCapacity; 
    }

    std::size_t size() const { 
        return mCount; 
    }

    std::size_t capacity() const { 
        return mCapacity; 
    }

    void clear(){
        mHead = 0;
        mTail = 0;
        mCount = 0;
    }

private:
    void advance(std::size_t& index){
        if (++index == mCapacity)
            index = 0;
    }

    T* mBuffer;
    std::size_t mCapacity;
    std::size_t mHead;
    std::size_t mTail;
    std::size_t mCount;

    // non-copyable
    HeapRingBuffer(const HeapRingBuffer&) = delete;
    HeapRingBuffer& operator=(const HeapRingBuffer&) = delete;
};

} // namespace Garbox
