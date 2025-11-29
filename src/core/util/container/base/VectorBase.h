#pragma once

#include <new>
#include "core/assert/Assert.h"

namespace Garbox {

/**
 * Fixed-capacity vector using custom storage.
 *
 * Supports:
 * - allocate()    => reserve uninitialized slot
 * - construct()   => default-construct element in-place
 * - assign()      => copy into uninitialized slot (no constructor)
 * - push()        => copy-construct element
 * - emplace()     => construct with args in-place
 * - destroyLast() => destroy last added elements
 * - destroyAll()  => destroy all elements
 * - releaseLast() => drop last added elements without destruction
 * - releaseAll()  => drop all elements without destruction
 * - at()          => get pointer or copy of element at index
 * - peekLast()    => get pointer or copy to last element
 * No heap allocations occur. Capacity is fixed by Storage.
 */
template <typename T, typename Storage>
class VectorBase : private Storage {
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

    VectorBase() : Storage(), mSize(0){
        // nothing to do
    }

    VectorBase(size_t elementCount) : Storage(elementCount), mSize(0){
        // nothing to do
    }

    // Allocate uninitialized slot
    T* allocate(){
        const bool full = (mSize >= Storage::capacityElements());
        AssertDebug(!full, "VectorBase", "allocate called when full");
        if(full){
            return nullptr;
        }

        T* ptr = Storage::elementPtr(mSize);
        mSize++;
        return ptr;
    }

    // Copy into uninitialized slot (no ctor)
    T* assign(const T& value){
        T* ptr = allocate();
        if(ptr != nullptr){
            *ptr = value;
        }
        return ptr;
    }

    // Default-construct into next slot
    T* construct(){
        T* ptr = allocate();
        if(ptr != nullptr){
            new(ptr) T();
        }
        return ptr;
    }

    // Copy-construct into next slot
    bool push(const T& value){
        const bool full = (mSize >= Storage::capacityElements());
        AssertDebug(!full, "VectorBase", "push called when full");
        if(full){
            return false;
        }

        new(Storage::elementPtr(mSize)) T(value);
        mSize++;
        return true;
    }

    // Construct with args in-place
    template<typename... Args>
    T* emplace(Args&&... args){
        const bool full = (mSize >= Storage::capacityElements());
        AssertDebug(!full, "VectorBase", "emplace called when full");
        if(full){
            return nullptr;
        }

        T* ptr = Storage::elementPtr(mSize);
        new(ptr) T(args...);
        mSize++;
        return ptr;
    }

    Iterator begin() {
        return Iterator(Storage::elementPtr(0));
    }

    Iterator end() {
        return Iterator(Storage::elementPtr(mSize));
    }

    T& operator[](std::size_t index) {
        return *Storage::elementPtr(index);
    }

    const T& operator[](std::size_t index) const {
        return *Storage::elementPtr(index);
    }

    std::size_t size() const {
        return mSize;
    }

    std::size_t capacity() const {
        return Storage::capacityElements();
    }

    bool full() const {
        return (mSize == Storage::capacityElements());
    }

    bool empty() const {
        return (mSize == 0);
    }

    // Destroy all constructed elements
    void destroyAll() {
        std::size_t i = 0;
        while(i < mSize){
            T* ptr = Storage::elementPtr(i);
            ptr->~T();
            i++;
        }
        mSize = 0;
    }

    // Drop everything without destruction
    void releaseAll(){
        mSize = 0;
    }

    // Destroy last element
    bool destroyLast(){
        if(mSize == 0){
            return false;
        }

        T* ptr = Storage::elementPtr(mSize - 1);
        ptr->~T();
        mSize--;
        return true;
    }

    // Destroy last element and copy it out
    bool destroyLast(T& out){
        if(mSize == 0){
            return false;
        }

        T* ptr = Storage::elementPtr(mSize - 1);
        out = *ptr;
        ptr->~T();
        mSize--;
        return true;
    }

    // Release last element (no destructor)
    bool releaseLast(){
        if(mSize == 0){
            return false;
        }

        mSize--;
        return true;
    }

    // Release last element, copy it out
    bool releaseLast(T& out){
        if(mSize == 0){
            return false;
        }

        T* ptr = Storage::elementPtr(mSize - 1);
        out = *ptr;

        mSize--;
        return true;
    }

    // Peek last element (copy)
    bool peekLast(T& out) const {
        if(mSize == 0){
            return false;
        }
        out = *Storage::elementPtr(mSize - 1);
        return true;
    }

    // Pointer to last element
    T* peekLast(){
        if(mSize == 0){
            return nullptr;
        }
        return Storage::elementPtr(mSize - 1);
    }

    // Copy element at index
    bool at(std::size_t index, T& out) const {
        const bool valid = (index < mSize);
        if(!valid){
            return false;
        }
        out = *Storage::elementPtr(index);
        return true;
    }

    // Pointer to element at index
    T* at(std::size_t index){
        const bool valid = (index < mSize);
        if(!valid){
            return nullptr;
        }
        return Storage::elementPtr(index);
    }

private:
    std::size_t mSize;
};

} // namespace Garbox
