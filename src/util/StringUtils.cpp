#include "StringUtils.h"

#include <cstdio>
#include <cinttypes>

namespace Garbox {

char* StringUtils::FormatDurationDHMS(uint32_t totalSeconds, char* out, size_t outSize){
    uint32_t days = totalSeconds / 86400U;
    totalSeconds %= 86400U;
    uint32_t hours = totalSeconds / 3600U;
    totalSeconds %= 3600U;
    uint32_t minutes = totalSeconds / 60U;
    uint32_t seconds = totalSeconds % 60U;

    snprintf(out, outSize, "%02" PRIu32 "d%02" PRIu32 "h%02" PRIu32 "m%02" PRIu32 "s",
        (uint32_t) days,
        (uint32_t) hours,
        (uint32_t) minutes,
        (uint32_t) seconds
    );

    return out;
}

} // namespace