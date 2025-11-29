#pragma once

#include "core/util/container/base/RingBufferBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template <typename T, std::size_t N>
class RingBufferStatic : public RingBufferBase<T, StaticStorage<T, N>> {
public:
    RingBufferStatic():  RingBufferBase<T, StaticStorage<T, N>>() {}
};

} // namespace Garbox
