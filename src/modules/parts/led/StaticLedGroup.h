#pragma once

#include <cstdint>
#include "modules/parts/led/single/StaticLed.h"
#include "core/util/container/Span.h"

namespace Garbox {

class StaticLedGroup {
public:
    StaticLedGroup(Span<StaticLed> staticLeds);
    ~StaticLedGroup();

    void init();

    StaticLed& getLed(uint8_t index);
    Garbox::Span<StaticLed>& getAllLeds();

    void setLed(uint8_t index, bool enabled);
    void setAllLeds(bool enabled);
    void toggleLed(uint8_t index);
    void toggleAllLeds();

    bool isInitialized() const;
    uint16_t getCount() const;

    // Disallow copy and move 
    StaticLedGroup(const StaticLedGroup&) = delete;
    StaticLedGroup& operator=(const StaticLedGroup&) = delete;
    StaticLedGroup(StaticLedGroup&&) = delete;
    StaticLedGroup& operator=(StaticLedGroup&&) = delete;

private:
    bool mInitialized = false;
    Span<StaticLed> mLeds;
};

} // namespace Garbox
