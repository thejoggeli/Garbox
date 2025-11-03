#include "TimeSlotScheduler.h"

#include "assert/Assert.h"
#include "core/time/Time.h"

namespace Garbox {

TimeSlotScheduler::TimeSlotScheduler(std::initializer_list<SlotConfig> configs){

    AssertExit(configs.size() > 0, "TimeSlotScheduler", "minimum one slot required");

    // determine maximum index to size mDiagnostics
    uint8_t maxIndex = 0;
    for (const SlotConfig& cfg : configs) {
        if (cfg.diagnosticsIndex > maxIndex){
            maxIndex = cfg.diagnosticsIndex;
        }
    }
    mDiagnostics.resize(maxIndex + 1);

    // initialize mDiagnostics
    for (Diagnostics& diagnostics : mDiagnostics) {
        diagnostics.maxTimeMicros = 0;
        diagnostics.maxTimeAllMicros = 0;
        diagnostics.frequencyCount = 0;
        diagnostics.frequencyHz = 0;
        diagnostics.minDurationMicros = UINT32_MAX; // start large so we can find min
    }

    // copy slot configurations
    mSlots.reserve(configs.size());
    for (const SlotConfig& cfg : configs) {
        mSlots.push_back({cfg.durationMicros, cfg.diagnosticsIndex, cfg.handler});

        // track minimum duration per diagnostics index
        Diagnostics& diag = mDiagnostics[cfg.diagnosticsIndex];
        if (cfg.durationMicros < diag.minDurationMicros) {
            diag.minDurationMicros = cfg.durationMicros;
        }

        // track maximum duration per diagnostics index
        if (cfg.durationMicros > diag.maxDurationMicros) {
            diag.maxDurationMicros = cfg.durationMicros;
        }
    }

    // normalize cases where no slots existed for some index
    for (Diagnostics& diagnostics : mDiagnostics) {
        if (diagnostics.minDurationMicros == UINT32_MAX){
            diagnostics.minDurationMicros = 0;
        }
    }    

}

void TimeSlotScheduler::run() {

    uint32_t currentTickTimeMicros = Time::GetMicros();

    if(mInitialRun){
        // initialize timing
        mNextSlotIndex = 0;
        mLastSlotDurationMicros = 0;
        mLastTickTimeMicros = 0;
        mLastFrequencyTimeMicros = currentTickTimeMicros;
        mInitialRun = false;
    }
    else if ((currentTickTimeMicros - mLastTickTimeMicros) < mLastSlotDurationMicros) {
        // wait until next slot is ready for execution
        return;
    }

    // get current slot and diagnostics
    Slot& slot = mSlots[mNextSlotIndex];
    Diagnostics& diagnostics = mDiagnostics[slot.diagnosticsIndex];

    // execute slot
    const uint32_t slotStartTimeMicros = currentTickTimeMicros;
    if (slot.handler) {
        slot.handler();
    }
    diagnostics.frequencyCount++;
    uint32_t slotEndTimeMicros = Time::GetMicros();

    // measure elapsed slot execution time 
    uint32_t slotTimeMicros = slotEndTimeMicros - slotStartTimeMicros;
    if (slotTimeMicros > diagnostics.maxTimeMicros){
        diagnostics.maxTimeMicros = slotTimeMicros;
        if (slotTimeMicros > diagnostics.maxTimeAllMicros){
            diagnostics.maxTimeAllMicros = slotTimeMicros;
        }
    }

    // advance to next slot
    mNextSlotIndex++;
    if (mNextSlotIndex >= mSlots.size()) {
        mNextSlotIndex = 0;

        // once per second snapshot and reset frequency counts
        if ((currentTickTimeMicros - mLastFrequencyTimeMicros) >= 1'000'000) {
            updateDiagnosticsFrequencies();
            mLastFrequencyTimeMicros = currentTickTimeMicros;
        }
    }

    // remember current timing for next tick
    mLastTickTimeMicros = currentTickTimeMicros;
    mLastSlotDurationMicros = slot.durationMicros;
}

void TimeSlotScheduler::updateDiagnosticsFrequencies(){
    for (Diagnostics& diag : mDiagnostics) {
        diag.frequencyHz = diag.frequencyCount;
        diag.frequencyCount = 0;
    }
}

const TimeSlotScheduler::Diagnostics& TimeSlotScheduler::getDiagnostics(uint8_t index) const {
    return mDiagnostics[index];
}

size_t TimeSlotScheduler::getSlotCount() const {
    return mSlots.size();
}

void TimeSlotScheduler::clearMaxTimes() {
    for (Diagnostics& diag : mDiagnostics) {
        diag.maxTimeMicros = 0;
    }
}

} // namespace