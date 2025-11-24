#pragma once

namespace Garbox {

template <typename Storage>
DataPoolBase<Storage>::DataPoolBase():
    // initialize members
    Storage(),
    mOffsetBytes(0){
    // nothing to do
}

template <typename Storage>
template<typename... Args>
DataPoolBase<Storage>::DataPoolBase(Args... storageArgs):
    // initialize members
    Storage(storageArgs...),
    mOffsetBytes(0){
    // nothing to do
}

template <typename Storage>
void DataPoolBase<Storage>::clear(){
    mOffsetBytes = 0;
}

template <typename Storage>
void* DataPoolBase<Storage>::allocateRaw(std::size_t sizeBytes, std::size_t alignmentBytes){
    const std::size_t mask = alignmentBytes - 1;
    const std::size_t alignedOffset = (mOffsetBytes + mask) & ~mask;

    const bool enoughSpace = (alignedOffset + sizeBytes) <= Storage::capacityBytes();
    if(!enoughSpace){
        TriggerDebug("DataPool", "insufficient space");
        return 0;
    }

    uint8_t* buffer = reinterpret_cast<uint8_t*>(Storage::dataBytes());
    uint8_t* ptr = buffer + alignedOffset;

    mOffsetBytes = alignedOffset + sizeBytes;
    return ptr;
}

template <typename Storage>
template<typename T>
T* DataPoolBase<Storage>::allocate(){
    return static_cast<T*>(allocateRaw(sizeof(T), alignof(T)));
}

template <typename Storage>
template<typename T>
T* DataPoolBase<Storage>::allocate(const T& data){
    T* ptr = allocate<T>();
    if(ptr != 0){
        *ptr = data;
    }
    return ptr;
}

template <typename Storage>
template<typename T>
T* DataPoolBase<Storage>::push(const T& value){
    void* rawPtr = allocateRaw(sizeof(T), alignof(T));
    if(rawPtr == 0){
        return 0;
    }

    T* ptr = new(rawPtr) T(value);
    return ptr;
}

template <typename Storage>
template<typename T, typename... Args>
T* DataPoolBase<Storage>::emplace(Args... args){
    void* rawPtr = allocateRaw(sizeof(T), alignof(T));
    if(rawPtr == 0){
        return 0;
    }

    T* ptr = new(rawPtr) T(args...);
    return ptr;
}

template <typename Storage>
std::size_t DataPoolBase<Storage>::capacity() const {
    return Storage::capacityBytes();
}

template <typename Storage>
std::size_t DataPoolBase<Storage>::used() const {
    return mOffsetBytes;
}

} // namespace Garbox
