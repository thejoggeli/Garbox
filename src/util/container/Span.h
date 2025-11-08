#pragma once

#include <array>
#include <cstddef>
#include "assert/Assert.h"

namespace Garbox {

// lightweight non-owning view over contiguous memory
template<typename T>
class Span {
public:
    // default constructor (creates empty span)
    Span():
        // initialize members
        mPtr(nullptr),
        mSize(0){
        // nothing to do
    }

    // construct from pointer and element count
    Span(T* ptr, std::size_t size):
        // initialize members
        mPtr(ptr),
        mSize(size){
        // nothing to do
    }

    // construct from raw C-style array
    template<std::size_t N>
    Span(T (&arr)[N]):
        // initialize members
        mPtr(arr),
        mSize(N){
        // nothing to do
    }

    // construct from std::array (non-const)
    template<std::size_t N>
    Span(std::array<T, N>& arr):
        // initialize members
        mPtr(arr.data()),
        mSize(N){
        // nothing to do
    }

    // construct from std::array (const)
    template<std::size_t N>
    Span(const std::array<T, N>& arr):
        // initialize members
        mPtr(arr.data()),
        mSize(N){
        // nothing to do
    }

    // iterator access
    T* begin(){ return mPtr; }
    T* end(){ return mPtr + mSize; }
    const T* begin() const { return mPtr; }
    const T* end() const { return mPtr + mSize; }

    // capacity queries
    std::size_t size() const { return mSize; }
    bool empty() const { return (mSize == 0); }

    // access element with bounds check (fatal on error)
    T& at(std::size_t index){
        const bool inRange = (index < mSize);
        AssertExit(inRange, "Span", "index out of range");
        return mPtr[index];
    }

    const T& at(std::size_t index) const {
        const bool inRange = (index < mSize);
        AssertExit(inRange, "Span", "index out of range");
        return mPtr[index];
    }

    // operator[] uses same safety check as at()
    T& operator[](std::size_t index){ return at(index); }
    const T& operator[](std::size_t index) const { return at(index); }

    // access first element (fatal if span empty)
    T& front(){
        const bool hasElements = (mSize > 0);
        AssertExit(hasElements, "Span", "front on empty span");
        return mPtr[0];
    }

    const T& front() const {
        const bool hasElements = (mSize > 0);
        AssertExit(hasElements, "Span", "front on empty span");
        return mPtr[0];
    }

    // access last element (fatal if span empty)
    T& back(){
        const bool hasElements = (mSize > 0);
        AssertExit(hasElements, "Span", "back on empty span");
        return mPtr[mSize - 1];
    }

    const T& back() const {
        const bool hasElements = (mSize > 0);
        AssertExit(hasElements, "Span", "back on empty span");
        return mPtr[mSize - 1];
    }

    // get raw pointer to underlying data
    T* data(){ return mPtr; }
    const T* data() const { return mPtr; }

private:
    // members
    T* mPtr = nullptr;
    std::size_t mSize = 0;
};

} // namespace Garbox
