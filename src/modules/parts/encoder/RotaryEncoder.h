#pragma once

#include <cstdint>
#include <driver/pcnt.h>

namespace Garbox {

class RotaryEncoder {
public:
    using Callback = void(*)(void* ctx, int32_t steps);

    struct Config {
        int32_t pinA = -1;
        int32_t pinB = -1;
        pcnt_unit_t unit = PCNT_UNIT_0;
    };

    RotaryEncoder();

    void init(const Config& config);
    void tick();

    void setCallback(void* ctx, Callback callback);

private:
    static int16_t wrapDelta(int16_t now, int16_t last);

    // configuration
    Callback mCallback = nullptr;
    void* mCallbackCtx = nullptr;

    // state
    bool mInitialized = false;

    int16_t mLastCount = 0;
};

} // namespace Garbox
