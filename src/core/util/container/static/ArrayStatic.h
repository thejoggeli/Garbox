#pragma once

#include "core/util/container/base/ArrayBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template<typename T, std::size_t N>
class ArrayStatic : public ArrayBase<T, StaticStorage<T, N>> {
public:
    ArrayStatic() = default;
};

} // namespace Garbox
