#pragma once
#include <cstdint>
#include <cstddef>

namespace Garbox {

template <typename T>
class HeapStack {
public:

    explicit HeapStack(std::size_t capacity): 
        // init members
        mBuffer(new T[capacity]),
        mCapacity(capacity),
        mCount(0){
        // nothing to do
    }

    ~HeapStack(){
        delete[] mBuffer;
    }

    bool push(const T& item){
        if (isFull())
            return false;
        mBuffer[mCount++] = item;
        return true;
    }

    bool pop(T& item){
        if (isEmpty())
            return false;
        item = mBuffer[--mCount];
        return true;
    }

    T* popPtr(){
        if (isEmpty())
            return nullptr;
        return &mBuffer[--mCount];
    }

    bool peek(T& item) const {
        if (isEmpty())
            return false;
        item = mBuffer[mCount - 1];
        return true;
    }

    const T* peekPtr() const {
        if (isEmpty())
            return nullptr;
        return &mBuffer[mCount - 1];
    }

    bool isEmpty() const { 
        return mCount == 0; 
    }

    bool isFull()  const { 
        return mCount == mCapacity; 
    }

    std::size_t size() const { 
        return mCount; 
    }
    
    std::size_t capacity() const { 
        return mCapacity; 
    }

    void clear(){ mCount = 0; }

private:
    T* mBuffer;
    std::size_t mCapacity;
    std::size_t mCount;

    // non-copyable
    HeapStack(const HeapStack&) = delete;
    HeapStack& operator=(const HeapStack&) = delete;
};

} // namespace Garbox
