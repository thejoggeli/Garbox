#pragma once

#include <atomic>

namespace Garbox {

class TaskStopToken {
public:
    bool stopRequested() const {
        return mRequested != nullptr
            && mRequested->load(std::memory_order_acquire);
    }

private:
    explicit TaskStopToken(const std::atomic<bool>* requested):
        mRequested(requested){}

    const std::atomic<bool>* mRequested = nullptr;

    friend class TaskStopSource;
};

class TaskStopSource {
public:
    TaskStopToken token() const {
        return TaskStopToken(&mRequested);
    }

    void requestStop(){
        mRequested.store(true, std::memory_order_release);
    }

    void reset(){
        mRequested.store(false, std::memory_order_release);
    }

    bool stopRequested() const {
        return mRequested.load(std::memory_order_acquire);
    }

private:
    std::atomic<bool> mRequested{false};
};

} // namespace Garbox
