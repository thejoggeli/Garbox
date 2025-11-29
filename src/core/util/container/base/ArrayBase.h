#pragma once

#include <cstddef>
#include <new>

namespace Garbox {

template<typename T, typename Storage>
class ArrayBase : private Storage {
public:

    class Iterator {
    public:
        Iterator(T* ptr) : mPtr(ptr) {}

        T& operator*()        { return *mPtr; }
        T* operator->()       { return  mPtr; }

        Iterator& operator++() {
            ++mPtr;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return mPtr != other.mPtr;
        }

    private:
        T* mPtr;
    };

    class ConstIterator {
    public:
        ConstIterator(const T* ptr) : mPtr(ptr) {}

        const T& operator*()  const { return *mPtr; }
        const T* operator->() const { return  mPtr; }

        ConstIterator& operator++() {
            ++mPtr;
            return *this;
        }

        bool operator!=(const ConstIterator& other) const {
            return mPtr != other.mPtr;
        }

    private:
        const T* mPtr;
    };

protected:

    struct without_count_t {};
    static constexpr without_count_t without_count{};

    struct with_count_t {};
    static constexpr with_count_t with_count{};

public:

    ArrayBase() : Storage(){
        constructAll();
    }

    ArrayBase(std::size_t elementCount) : Storage(elementCount){
        constructAll();
    }

    template<typename... Args>
    ArrayBase(without_count_t, Args&&... args) : Storage(){
        constructAllWith(std::forward<Args>(args)...);
    }

    template<typename... Args>
    ArrayBase(with_count_t, std::size_t elementCount, Args&&... args) : Storage(elementCount){
        constructAllWith(std::forward<Args>(args)...);
    }

    T& operator[](std::size_t index) {
        return *Storage::elementPtr(index);
    }

    const T& operator[](std::size_t index) const {
        return *Storage::elementPtr(index);
    }

    T* data() {
        return Storage::dataElements();
    }

    const T* data() const {
        return Storage::dataElements();
    }

    std::size_t size() const {
        return Storage::capacityElements();
    }

    std::size_t capacity() const {
        return Storage::capacityElements();
    }

    Iterator begin() {
        return Iterator(Storage::dataElements());
    }

    Iterator end() {
        return Iterator(Storage::dataElements() + size());
    }

    ConstIterator begin() const {
        return ConstIterator(Storage::dataElements());
    }

    ConstIterator end() const {
        return ConstIterator(Storage::dataElements() + size());
    }

    ConstIterator cbegin() const {
        return ConstIterator(Storage::dataElements());
    }

    ConstIterator cend() const {
        return ConstIterator(Storage::dataElements() + size());
    }

private:

    void constructAll() {
        const std::size_t n = Storage::capacityElements();
        for(std::size_t i = 0; i < n; i++) {
            new (Storage::elementPtr(i)) T();
        }
    }

    template<typename... Args>
    void constructAllWith(Args&&... args) {
        const std::size_t n = Storage::capacityElements();
        for(std::size_t i = 0; i < n; i++) {
            new (Storage::elementPtr(i)) T(std::forward<Args>(args)...);
        }
    }

};

} // namespace Garbox
