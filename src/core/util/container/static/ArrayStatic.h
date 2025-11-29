#pragma once

#include "core/util/container/base/ArrayBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template<typename T, std::size_t N>
class ArrayStatic : public ArrayBase<T, StaticStorage<T, N>> {
public:

    ArrayStatic() : ArrayBase<T, StaticStorage<T, N>>() {}

    template<typename... Args>
    ArrayStatic(Args&&... args) : ArrayBase<T, StaticStorage<T, N>>(
        ArrayBase<T, StaticStorage<T, N>>::without_count,
        std::forward<Args>(args)...) {}

};

} // namespace Garbox
