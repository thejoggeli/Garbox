#pragma once

#include <array>
#include <atomic>
#include <cstddef>

namespace Garbox {

template<std::size_t Capacity>
class SafeOutputRegistry {
public:
    using Handler = void (*)(void* context);

    bool registerHandler(Handler handler, void* context){
        if(handler == nullptr
           || mApplied.load(std::memory_order_acquire)
           || mCount >= Capacity){
            return false;
        }
        mEntries[mCount++] = {handler, context};
        return true;
    }

    bool apply(){
        bool expected = false;
        if(!mApplied.compare_exchange_strong(
               expected, true,
               std::memory_order_acq_rel,
               std::memory_order_acquire)){
            return false;
        }

        for(std::size_t i = 0; i < mCount; ++i){
            mEntries[i].handler(mEntries[i].context);
        }
        return true;
    }

    bool applied() const {
        return mApplied.load(std::memory_order_acquire);
    }

    std::size_t size() const {
        return mCount;
    }

private:
    struct Entry {
        Handler handler = nullptr;
        void* context = nullptr;
    };

    std::array<Entry, Capacity> mEntries{};
    std::size_t mCount = 0U;
    std::atomic<bool> mApplied{false};
};

} // namespace Garbox
