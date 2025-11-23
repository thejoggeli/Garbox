#pragma once

#include <cstddef>
#include <new>

#include "core/assert/Assert.h"

namespace Garbox {

template <typename T, std::size_t N>
class StaticVector {
public:

    // iterator for range based for loops
    class Iterator {
    public:

        Iterator(T* ptr):
            // initialize members
            mPtr(ptr){
            // nothing to do
        }

        T& operator*() {
            return mPtr[0];
        }

        T* operator->() {
            return mPtr;
        }

        Iterator& operator++() {
            mPtr++;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return (mPtr != other.mPtr);
        }

    private:
        T* mPtr;
    };

public:

    StaticVector():
        // initialize members
        mSize(0){
        // nothing to do
    }

    bool push(const T& value){
        const bool isFull = (mSize >= N);
        AssertDebug(!isFull, "StaticVector", "push called when full");
        if(isFull){
            return false;
        }

        // placement new: copy-construct into raw storage
        new(&mStorage[mSize * sizeof(T)]) T(value);
        mSize++;
        return true;
    }

    // construct element in-place, no copying
    template<typename... Args>
    T* emplace(Args&&... args){
        const bool isFull = (mSize >= N);
        AssertDebug(!isFull, "StaticVector", "emplace called when full");
        if(isFull){
            return 0;
        }

        T* ptr = reinterpret_cast<T*>(&mStorage[mSize * sizeof(T)]);
        new(ptr) T(args...);
        mSize++;
        return ptr;
    }

    Iterator begin() {
        return Iterator(reinterpret_cast<T*>(&mStorage[0]));
    }

    Iterator end() {
        return Iterator(reinterpret_cast<T*>(&mStorage[mSize * sizeof(T)]));
    }

    T& operator[](std::size_t index) {
        return *(reinterpret_cast<T*>(&mStorage[index * sizeof(T)]));
    }

    const T& operator[](std::size_t index) const {
        return *(reinterpret_cast<const T*>(&mStorage[index * sizeof(T)]));
    }

    std::size_t size() const {
        return mSize;
    }

    std::size_t capacity() const {
        return N;
    }

    bool full() const {
        return (mSize == N);
    }

    bool empty() const {
        return (mSize == 0);
    }

    void clear() {
        // manually call destructors
        std::size_t i = 0;
        while(i < mSize){
            T* ptr = reinterpret_cast<T*>(&mStorage[i * sizeof(T)]);
            ptr->~T();
            i++;
        }
        mSize = 0;
    }

private:
    // raw uninitialized storage
    alignas(T) unsigned char mStorage[N * sizeof(T)];
    std::size_t mSize;
};

} // namespace Garbox
