#pragma once

#include "core/util/container/base/MaskedArrayBase.h"
#include "core/util/container/storage/StaticStorage.h"
#include "core/util/container/static/ArrayStatic.h"

namespace Garbox {

template<typename T, std::size_t N>
class MaskedArrayStatic : public MaskedArrayBase<T, StaticStorage<T, N>, ArrayStatic<size_t, N>, ArrayStatic<bool, N>> {
public:
    MaskedArrayStatic(bool defaultEnabled) : MaskedArrayBase<T, StaticStorage<T, N>, ArrayStatic<size_t, N>, ArrayStatic<bool, N>>(defaultEnabled) {}
};

} // namespace Garbox
