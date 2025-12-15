#pragma once

#include <cstdint>
#include <cstddef>

namespace Garbox {

class StringUtils {
public:

    // Prevent instantiation
    StringUtils() = delete;

    static char* FormatDurationDHMS(uint32_t totalSeconds, char* out, size_t outSize, const char* spacer = " ");

};

} // namespace
