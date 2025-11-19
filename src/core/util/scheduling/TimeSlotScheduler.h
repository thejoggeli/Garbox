#pragma once

#include <vector>
#include <cstdint>
#include <initializer_list>

namespace Garbox {

class TimeSlotScheduler {
public:

    using SlotHandler = void (*)();

    struct SlotConfig {
        uint32_t durationMicros;  // length of time slot for this task
        uint8_t diagnosticsIndex; // used to group and retrieve diagnostic data
        SlotHandler handler;      // function pointer (nullptr = idle)
    };

    struct Diagnostics {
        int32_t maxTimeMicros;      // max measured execution time for any task of this index (since last clear)
        int32_t maxTimeAllMicros;   // max measured execution time for any task of this index (since start)
        uint32_t frequencyCount;    // counter to measure frequency
        uint32_t frequencyHz;       // measured frequency, updated every second
        uint32_t minDurationMicros; // min(durationMicros) of all slots with this diagnostics index
        uint32_t maxDurationMicros; // max(durationMicros) of all slots with this diagnostics index
    };

    explicit TimeSlotScheduler(std::initializer_list<SlotConfig> configs);

    void run();

    const Diagnostics& getDiagnostics(uint8_t index) const;
    size_t getSlotCount() const;
    void clearMaxTimes();

private:

    void updateDiagnosticsFrequencies();

    struct Slot {
        uint32_t durationMicros;
        uint8_t diagnosticsIndex;
        SlotHandler handler;
    };

    uint8_t mNextSlotIndex = 0;
    uint32_t mLastTickTimeMicros = 0;
    uint32_t mLastSlotDurationMicros = 0;
    uint32_t mLastFrequencyTimeMicros = 0;

    bool mInitialRun = true;

    std::vector<Slot> mSlots;
    std::vector<Diagnostics> mDiagnostics;
};

} // namespace