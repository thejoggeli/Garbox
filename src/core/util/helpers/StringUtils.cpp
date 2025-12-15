#include "StringUtils.h"

#include <cstdio>
#include <cinttypes>

namespace Garbox {

char* StringUtils::FormatDurationDHMS(uint32_t totalSeconds, char* out, size_t outSize, const char* spacer){
    uint32_t days = totalSeconds / 86400U;
    totalSeconds %= 86400U;
    uint32_t hours = totalSeconds / 3600U;
    totalSeconds %= 3600U;
    uint32_t minutes = totalSeconds / 60U;
    uint32_t seconds = totalSeconds % 60U;

    snprintf(out, outSize, "%02" PRIu32 "d%s%02" PRIu32 "h%s%02" PRIu32 "m%s%02" PRIu32 "s",
        (uint32_t) days, spacer,
        (uint32_t) hours, spacer,
        (uint32_t) minutes, spacer,
        (uint32_t) seconds
    );

    return out;
}

} // namespace