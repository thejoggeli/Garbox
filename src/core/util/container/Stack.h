#pragma once

#include <array>
#include <cstddef>
#include "core/assert/Assert.h"

namespace Garbox {

// lightweight non-owning view over contiguous memory
template<typename T>
class Span {
public:
    // default constructor (creates empty span)
    Span();

    // construct from pointer and element count
    Span(T* ptr, std::size_t size);

    // construct from raw C-style array
    template<std::size_t N>
    Span(T (&arr)[N]);

    // construct from std::array (non-const)
    template<std::size_t N>
    Span(std::array<T, N>& arr);

    // construct from std::array (const)
    template<std::size_t N>
    Span(const std::array<T, N>& arr);

    // iterator access
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    // capacity queries
    std::size_t size() const;
    bool empty() const;

    // element access (fatal on invalid index)
    T& at(std::size_t index);
    const T& at(std::size_t index) const;

    // operator[] uses same safety check as at()
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    // access first element (fatal if span empty)
    T& front();
    const T& front() const;

    // access last element (fatal if span empty)
    T& back();
    const T& back() const;

    // get raw pointer to underlying data
    T* data();
    const T* data() const;

private:
    // members
    T* mPtr = nullptr;
    std::size_t mSize = 0;
};

} // namespace Garbox

// include template implementation
#include "Span.tpp"
