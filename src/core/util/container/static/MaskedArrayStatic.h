#pragma once

#include "core/util/container/base/MaskedArrayBase.h"
#include "core/util/container/storage/StaticStorage.h"

namespace Garbox {

template<typename T, std::size_t N>
class MaskedArrayStatic : public MaskedArrayBase<T, StaticStorage<T, N>> {
public:
    MaskedArrayStatic() : MaskedArrayStatic<T, StaticStorage<T, N>>() {}
};

} // namespace Garbox
