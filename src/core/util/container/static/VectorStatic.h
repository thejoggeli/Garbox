#pragma once

#include "core/util/container/storage/StaticStorage.h"
#include "core/util/container/base/VectorBase.h"

namespace Garbox {

template <typename T, std::size_t N>
class VectorStatic : public VectorBase<T, StaticStorage<T, N>> {
public:
    VectorStatic() : VectorBase<T, StaticStorage<T, N>>() {}
};

} // namespace Garbox
