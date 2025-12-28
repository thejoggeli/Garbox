#pragma once

#include "core/util/container/heap/VectorHeap.h"

namespace Garbox {

class TickRunner {
public:

    using Handler = void (*)(void* ctx);

    struct TickPhase {
        Handler handler;
        uint32_t delayMillis = 0;        
    };

    TickRunner(size_t maxTickPhaseHandlers, uint32_t periodMillis, void* callbackCtx);

    void setTickStartHandler(Handler handler);
    void setTickEndHandler(Handler handler);
    void registerTickPhase(Handler handler, uint32_t delayMillis);
    void run();

private:

    VectorHeap<TickPhase> mTickPhaseHandlers;
    Handler mTickStartHandler = nullptr;
    Handler mTickEndHandler = nullptr;

    uint32_t mPeriodMillis;
    uint32_t mRemainingDelay;

    void* mCallbackCtx;

};

} // namespace