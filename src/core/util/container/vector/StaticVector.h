#pragma once

#include "core/util/container/storage/StaticStorage.h"
#include "core/util/container/vector/VectorBase.h"

namespace Garbox {

template <typename T, std::size_t N>
class StaticVector : public VectorBase<T, StaticStorage<T, N>> {
public:

    StaticVector():
        // initialize members
        VectorBase<T, StaticStorage<T, N>>() {
        // nothing to do
    }
};

} // namespace Garbox
