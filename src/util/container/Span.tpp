#pragma once

#include "Span.h"

namespace Garbox {

// default constructor (creates empty span)
template<typename T>
Span<T>::Span():
    // initialize members
    mPtr(nullptr),
    mSize(0){
    // nothing to do
}

// construct from pointer and element count
template<typename T>
Span<T>::Span(T* ptr, std::size_t size):
    // initialize members
    mPtr(ptr),
    mSize(size){
    // nothing to do
}

// construct from raw C-style array
template<typename T>
template<std::size_t N>
Span<T>::Span(T (&arr)[N]):
    // initialize members
    mPtr(arr),
    mSize(N){
    // nothing to do
}

// construct from std::array (non-const)
template<typename T>
template<std::size_t N>
Span<T>::Span(std::array<T, N>& arr):
    // initialize members
    mPtr(arr.data()),
    mSize(N){
    // nothing to do
}

// construct from std::array (const)
template<typename T>
template<std::size_t N>
Span<T>::Span(const std::array<T, N>& arr):
    // initialize members
    mPtr(arr.data()),
    mSize(N){
    // nothing to do
}

// iterator access
template<typename T>
T* Span<T>::begin(){
    return mPtr;
}

template<typename T>
T* Span<T>::end(){
    return mPtr + mSize;
}

template<typename T>
const T* Span<T>::begin() const {
    return mPtr;
}

template<typename T>
const T* Span<T>::end() const {
    return mPtr + mSize;
}

// capacity queries
template<typename T>
std::size_t Span<T>::size() const {
    return mSize;
}

template<typename T>
bool Span<T>::empty() const {
    return (mSize == 0);
}

// element access (fatal on invalid index)
template<typename T>
T& Span<T>::at(std::size_t index){
    const bool inRange = (index < mSize);
    AssertExit(inRange, "Span", "index out of range");
    return mPtr[index];
}

template<typename T>
const T& Span<T>::at(std::size_t index) const {
    const bool inRange = (index < mSize);
    AssertExit(inRange, "Span", "index out of range");
    return mPtr[index];
}

// operator[] uses same safety check as at()
template<typename T>
T& Span<T>::operator[](std::size_t index){
    return at(index);
}

template<typename T>
const T& Span<T>::operator[](std::size_t index) const {
    return at(index);
}

// access first element (fatal if span empty)
template<typename T>
T& Span<T>::front(){
    const bool hasElements = (mSize > 0);
    AssertExit(hasElements, "Span", "front on empty span");
    return mPtr[0];
}

template<typename T>
const T& Span<T>::front() const {
    const bool hasElements = (mSize > 0);
    AssertExit(hasElements, "Span", "front on empty span");
    return mPtr[0];
}

// access last element (fatal if span empty)
template<typename T>
T& Span<T>::back(){
    const bool hasElements = (mSize > 0);
    AssertExit(hasElements, "Span", "back on empty span");
    return mPtr[mSize - 1];
}

template<typename T>
const T& Span<T>::back() const {
    const bool hasElements = (mSize > 0);
    AssertExit(hasElements, "Span", "back on empty span");
    return mPtr[mSize - 1];
}

// get raw pointer to underlying data
template<typename T>
T* Span<T>::data(){
    return mPtr;
}

template<typename T>
const T* Span<T>::data() const {
    return mPtr;
}

} // namespace Garbox
