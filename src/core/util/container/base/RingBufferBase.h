#pragma once

#include <new>
#include "core/assert/Assert.h"

namespace Garbox {

/**
 * Double-ended fixed-capacity ring buffer.
 *
 * Elements can be added or removed at both front and back.
 * Supports:
 * - allocate()  => allocate uninitialized slots
 * - construct() => in-place construction
 * - push()      => copy construction
 * - emplace()   => move construction
 * - peek()      => get either pointer or copy of front/back element
 * - destroy()   => remove and call destructor
 * - release()   => remove without calling destructor. Optionally a pointer can be passed to access the (still valid) element.
 */
template <typename T, typename Storage>
class RingBufferBase : private Storage {
public:

    RingBufferBase():
        // initialize members
        Storage(),
        mFront(0),
        mBack(0),
        mCount(0){
        // nothing to do
    }

    template<typename... Args>
    RingBufferBase(Args... storageArgs):
        // initialize members
        Storage(storageArgs...),
        mFront(0),
        mBack(0),
        mCount(0){
        // nothing to do
    }

    // Get number of elements stored
    std::size_t size() const {
        return mCount;
    }

    // Get maximum capacity
    std::size_t capacity() const {
        return Storage::capacityElements();
    }

    // Return true if empty
    bool isEmpty() const {
        return (mCount == 0);
    }

    // Return true if full
    bool isFull() const {
        return (mCount == Storage::capacityElements());
    }

    // Available space
    std::size_t available() const {
        return Storage::capacityElements() - mCount;
    }

    // Reset without calling destructors
    void releaseAll(){
        mFront = 0;
        mBack = 0;
        mCount = 0;
    }

    // Destroy all constructed elements
    void destroyAll(){
        T* buffer = Storage::dataElements();

        std::size_t index = mFront;
        std::size_t destroyed = 0;

        while(destroyed < mCount){
            buffer[index].~T();
            index = nextIndex(index);
            destroyed++;
        }
        mFront = 0;
        mBack = 0;
        mCount = 0;
    }

    // Insert raw (uninitialized) element at front
    T* allocateFront(){
        if(isFull()){
            return 0;
        }

        mFront = prevIndex(mFront);
        T* buffer = Storage::dataElements();
        T* slot = &buffer[mFront];

        mCount++;
        return slot;
    }

    // Insert raw (uninitialized) element at back
    T* allocateBack(){
        if(isFull()){
            return 0;
        }

        T* buffer = Storage::dataElements();
        T* slot = &buffer[mBack];

        mBack = nextIndex(mBack);
        mCount++;
        return slot;
    }

    // Insert a new element at the front using copy construction.
    T* pushFront(const T& value){
        return emplaceFront(value);
    }

    // Insert a new element at the back using copy construction.
    T* pushBack(const T& value){
        return emplaceBack(value);
    }

    // Default-construct at front
    T* constructFront(){
        T* slot = allocateFront();
        if(slot != 0){
            new(slot) T();
        }
        return slot;
    }

    // Default-construct at back
    T* constructBack(){
        T* slot = allocateBack();
        if(slot != 0){
            new(slot) T();
        }
        return slot;
    }

    // Construct with args at front
    template<typename... Args>
    T* emplaceFront(Args... args){
        T* slot = allocateFront();
        if(slot != 0){
            new(slot) T(args...);
        }
        return slot;
    }

    // Construct with args at back
    template<typename... Args>
    T* emplaceBack(Args... args){
        T* slot = allocateBack();
        if(slot != 0){
            new(slot) T(args...);
        }
        return slot;
    }

    // Pop from front and destroy
    bool destroyFront(){
        if(isEmpty()){
            return false;
        }

        T* buffer = Storage::dataElements();
        buffer[mFront].~T();

        mFront = nextIndex(mFront);
        mCount--;
        return true;
    }

    // Pop from front and output then destroy
    bool destroyFront(T& out){
        if(isEmpty()){
            return false;
        }

        T* buffer = Storage::dataElements();
        out = buffer[mFront];

        buffer[mFront].~T();

        mFront = nextIndex(mFront);
        mCount--;
        return true;
    }

    // Pop from front without destructor
    bool releaseFront(){
        if(isEmpty()){
            return false;
        }

        mFront = nextIndex(mFront);
        mCount--;
        return true;
    }

    // Pop from front without destructor and provide pointer to (still valid) element
    bool releaseFront(T*& ptr){
        if(isEmpty()){
            return false;
        }
        
        T* buffer = Storage::dataElements();
        ptr = &buffer[mFront];

        mFront = nextIndex(mFront);
        mCount--;
        return true;
    }

    bool releaseFront(T& out){
        if(isEmpty()){
            return false;
        }

        T* buffer = Storage::dataElements();
        out = buffer[mFront];

        mFront = nextIndex(mFront);
        mCount--;
        return true;
    }

    // Pop from back and destroy
    bool destroyBack(){
        if(isEmpty()){
            return false;
        }

        mBack = prevIndex(mBack);

        T* buffer = Storage::dataElements();
        buffer[mBack].~T();

        mCount--;
        return true;
    }

    // Pop from back and output then destroy
    bool destroyBack(T& out){
        if(isEmpty()){
            return false;
        }

        mBack = prevIndex(mBack);

        T* buffer = Storage::dataElements();
        out = buffer[mBack];

        buffer[mBack].~T();

        mCount--;
        return true;
    }

    // Pop from back without destructor
    bool releaseBack(){
        if(isEmpty()){
            return false;
        }

        mBack = prevIndex(mBack);
        mCount--;
        return true;
    }

    // Pop from back without destructor and provide pointer to (still valid) element
    bool releaseBack(T*& ptr){
        if(isEmpty()){
            return false;
        }

        mBack = prevIndex(mBack);

        T* buffer = Storage::dataElements();
        ptr = &buffer[mBack];

        mCount--;
        return true;
    }

    bool releaseBack(T& out){
        if(isEmpty()){
            return false;
        }

        mBack = prevIndex(mBack);

        T* buffer = Storage::dataElements();
        out = buffer[mBack];

        mCount--;
        return true;
    }

    // Peek front (copy)
    bool peekFront(T& out) const {
        if(isEmpty()){
            return false;
        }

        const T* buffer = Storage::dataElements();
        out = buffer[mFront];
        return true;
    }

    // Peek back (copy)
    bool peekBack(T& out) const {
        if(isEmpty()){
            return false;
        }

        const std::size_t last = prevIndex(mBack);
        const T* buffer = Storage::dataElements();
        out = buffer[last];
        return true;
    }

    // Pointer to element at front
    T* peekFront(){
        if(isEmpty()){
            return 0;
        }

        T* buffer = Storage::dataElements();
        return &buffer[mFront];
    }

    // Pointer to element at back
    T* peekBack(){
        if(isEmpty()){
            return 0;
        }

        const std::size_t last = prevIndex(mBack);
        T* buffer = Storage::dataElements();
        return &buffer[last];
    }

private:

    std::size_t nextIndex(std::size_t index) const {
        index++;
        if(index == Storage::capacityElements()){
            index = 0;
        }
        return index;
    }

    std::size_t prevIndex(std::size_t index) const {
        if(index == 0){
            index = Storage::capacityElements() - 1;
        }
        else {
            index--;
        }
        return index;
    }

private:
    std::size_t mFront;
    std::size_t mBack;
    std::size_t mCount;
};

} // namespace Garbox
