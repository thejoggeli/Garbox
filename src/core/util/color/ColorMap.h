#pragma once

#include <cstddef>
#include "core/util/color/types/HslColor.h"
#include "core/util/color/types/LabColor.h"
#include "core/util/color/types/RgbFloat.h"
#include "core/util/container/Span.h"

namespace Garbox {

/*
    Lightweight non-owning interpolation helper.
    The user supplies an external array of Entries. Each entry contains t and
    precomputed representations in multiple color spaces. The class uses a
    small lookup table for faster segment resolution and supports interpolation
    in standard RGB, linear RGB, HSL and Lab.
*/
class ColorMap {
public:

    struct Entry {
        const float t;
        const RgbFloat standardRgb;
        const RgbFloat linearRgb;
        const HslColor hsl;
        const LabColor lab;

        // t distribution set by user
        Entry(float t, const HslColor& hslColor);
        Entry(float t, const RgbFloat& rgbColor); // expects values in standard RGB (sRGB) format

        // t distributed uniformly
        Entry(const HslColor& hslColor);
        Entry(const RgbFloat& rgbColor); // expects values in standard RGB (sRGB) format
    };

    ColorMap(const Span<const Entry> entries);
    ~ColorMap();

    const Entry& getEntry(size_t index) const;
    size_t size() const;

    RgbFloat interpolateStandardRgb(float t) const;
    RgbFloat interpolateLinearRgb(float t) const;
    HslColor interpolateHsl(float t) const;
    LabColor interpolateLab(float t) const; 

    // Disallow copy and move 
    ColorMap(const ColorMap&) = delete;
    ColorMap& operator=(const ColorMap&) = delete;
    ColorMap(ColorMap&&) = delete;
    ColorMap& operator=(ColorMap&&) = delete;

private:

    struct LookupBucket {
        uint8_t startIndex;
        uint8_t endIndex;
        uint8_t size;
        void set(uint8_t startIndex, uint8_t endIndex);
    };

    struct SegmentInfo {
        const Entry* a;
        const Entry* b;
        float frac;
    };

    static constexpr size_t BucketCount = 16U;

    const Span<const Entry> mEntries;

    LookupBucket mLookup[BucketCount];
    bool mInitialized = false;
    bool mUniform = false;
    float mUniformStep = 0.0f;

    // initialization 
    void init();
    bool checkUniform() const ;
    void buildLookupBuckets();

    // interpolation
    void resolveSegmentUniform(float tClamped, SegmentInfo* segmentInfo) const;
    void resolveSegmentNonUniform(float tClamped, SegmentInfo* segmentInfo) const;
    void resolveSegment(float t, SegmentInfo* segmentInfo) const;
};

} // namespace Garbox
