#pragma once

#include <cstdint>

namespace Garbox {

class Timer {
public:

    Timer();
    
    void start(uint32_t duration);
    void restart();
    void extend (uint32_t duration);
    bool isExpired();

private:

    uint32_t mStartTimeMillis = 0;
    uint32_t mDurationMillis = 0;

};

} // namespace