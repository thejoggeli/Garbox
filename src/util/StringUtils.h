#pragma once

#include <cstdint>
#include <cstddef>

namespace Garbox {

class StringUtils {
public:

    static char* FormatDurationDHMS(uint32_t totalSeconds, char* out, size_t outSize);

};

} // namespace
