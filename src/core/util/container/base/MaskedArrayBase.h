#pragma once

#include <cstddef>
#include "core/assert/Assert.h"

namespace Garbox {

/**
 * Fixed-capacity array where each index can be enabled or disabled.
 * Enabled indices form an O(1) doubly-linked active chain for fast iteration.
 * Elements are always present; masking only affects iteration and lookup.
 */
template<typename T, typename Storage, typename IndexStorage, typename BoolStorage>
class MaskedArrayBase : private Storage {
public:
    static constexpr std::size_t INVALID = static_cast<std::size_t>(-1);

    class Iterator {
    public:
        Iterator(T* data, const std::size_t* next, std::size_t idx) : mData(data), mNext(next), mIdx(idx){}

        T& operator*(){ return mData[mIdx]; }
        T* operator->(){ return &mData[mIdx]; }

        Iterator& operator++(){
            mIdx = mNext[mIdx];
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return mIdx != other.mIdx;
        }

        size_t index() const {
            return mIdx;
        }

    private:
        T* mData;
        const std::size_t* mNext;
        std::size_t mIdx;
    };

    MaskedArrayBase(bool defaultEnabled): Storage(){
        initElements();
        initLinks(defaultEnabled);
    }

    MaskedArrayBase(size_t elementCount, bool defaultEnabled): 
        Storage(elementCount),
        mPrev(elementCount),
        mNext(elementCount),
        mEnabled(elementCount){
        initElements();
        initLinks(defaultEnabled);
    }

    Iterator begin(){
        return Iterator(Storage::elementPtr(0), mNext.data(), mHead);
    }

    Iterator end(){
        return Iterator(Storage::elementPtr(0), mNext.data(), INVALID);
    }

    T& at(std::size_t index) {
        return *Storage::elementPtr(index);
    }

    const T& at(std::size_t index) const {
        return *Storage::elementPtr(index);
    }

    T& operator[](std::size_t index){
        return *Storage::elementPtr(index);
    }

    const T& operator[](std::size_t index) const {
        return *Storage::elementPtr(index);
    }

    bool isIndexEnabled(std::size_t index) const {
        return mEnabled[index];
    }

    void setIndexEnabled(std::size_t index, bool enable){
        AssertDebug(index < capacity(), "MaskedArrayBase", "index out of range");

        if(enable){
            if(mEnabled[index]) return;
            enableIndex(index);
        } else {
            if(!mEnabled[index]) return;
            disableIndex(index);
        }
    }

    std::size_t capacity() const {
        return Storage::capacityElements();
    }

private:

    void initElements(){
        for(std::size_t i = 0; i < capacity(); i++){
            new(Storage::elementPtr(i)) T();
        }
    }

    void initLinks(bool defaultEnabled){
        std::size_t n = capacity();
        if(defaultEnabled){
            mHead = (n > 0) ? 0 : INVALID;
            for(std::size_t i = 0; i < n; i++){
                mPrev[i] = (i == 0) ? INVALID : (i - 1);
                mNext[i] = (i == n - 1) ? INVALID : (i + 1);
                mEnabled[i] = true;
            }
        } else {
            mHead = INVALID;
            for(std::size_t i = 0; i < n; i++){
                mPrev[i] = INVALID;
                mNext[i] = INVALID;
                mEnabled[i] = false;
            }
        }
    }

    void disableIndex(std::size_t i) {
        std::size_t p = mPrev[i];
        std::size_t n = mNext[i];

        // unlink from active chain
        if(p != INVALID)
            mNext[p] = n;
        else
            mHead = n;

        if(n != INVALID)
            mPrev[n] = p;

        // clear links for i
        mPrev[i] = INVALID;
        mNext[i] = INVALID;
        mEnabled[i] = false;
    }

    void enableIndex(std::size_t i) {
        std::size_t N = capacity();

        // find left active neighbor
        std::size_t p = (i == 0 ? INVALID : i - 1);
        while(p != INVALID && !mEnabled[p]) {
            if(p == 0) { p = INVALID; break; }
            p--;
        }

        // find right active neighbor
        std::size_t n = (i + 1 >= N ? INVALID : i + 1);
        while(n != INVALID && !mEnabled[n]) {
            if(n + 1 >= N) { n = INVALID; break; }
            n++;
        }

        // insert i between p and n
        mPrev[i] = p;
        mNext[i] = n;

        if(p != INVALID)
            mNext[p] = i;
        else
            mHead = i;

        if(n != INVALID)
            mPrev[n] = i;

        mEnabled[i] = true;
    }

    IndexStorage mPrev;
    IndexStorage mNext;
    BoolStorage mEnabled;
    std::size_t mHead;
};

} // namespace Garbox
